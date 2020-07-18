#include "optimization/ransac.h"

#include <Eigen/Dense>
#include <algorithm>
#include <random>

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "glog/stl_logging.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "optimization/ransac_impl.h"

DEFINE_int32(num_test_cases, 10, "Number of test cases to run.");

namespace dungeon {

// A model that fits a straight line to observations in 2D.
class LinearModel : public RansacModelBase {
 public:
  // Given that each observation is a vector in R^2, fits parameters (m, b)
  // to minimize L2 error:
  //   \sum_i || m*x(i, 0) + b - x(i, 1) ||^2
  virtual void Fit(const std::vector<Eigen::MatrixXf>& observations) override {
    Eigen::MatrixXf A(observations.size(), 2);
    Eigen::MatrixXf b(observations.size(), 1);
    for (size_t i = 0; i < observations.size(); ++i) {
      const Eigen::MatrixXf& obs = observations[i];
      CHECK_EQ(2, obs.rows());
      CHECK_EQ(1, obs.cols());

      A(i, 0) = obs(0, 0);
      A(i, 1) = 1.0;
      b(i, 0) = obs(1, 0);
    }
    params_ = A.colPivHouseholderQr().solve(b);
    CHECK_EQ(2, params_.rows());
    CHECK_EQ(1, params_.cols());
  }

  // Returns abs(m*x(0) + b - x(1)) for a given observation x.
  virtual double EvaluateError(const Eigen::MatrixXf& pt) const override {
    CHECK_EQ(2, pt.rows());
    CHECK_EQ(1, pt.cols());
    float x = pt(0, 0);
    float y = pt(1, 0);
    return std::abs(params_(0, 0) * x + params_(1, 0) - y);
  }

  virtual Eigen::MatrixXf params() const override { return params_; }

  // Returns the minimum number of observations needed to fit the model.
  virtual int min_num_observations() const { return 2; }

 private:
  Eigen::MatrixXf params_;
};

// Returns a collection of observations for the above model.
std::vector<Eigen::MatrixXf> CreateObservationCollection(
    const Eigen::MatrixXf& params, int num_observations) {
  CHECK_EQ(2, params.rows());
  CHECK_EQ(1, params.cols());
  const float params_x = params(0, 0);
  const float params_y = params(1, 0);
  std::vector<Eigen::MatrixXf> output;
  for (int i = 0; i < num_observations; ++i) {
    float val = drand48() * 10.0 - 5.0;
    output.push_back(
        Eigen::MatrixXf(Eigen::Vector2f(val, params_x * val + params_y)));
  }
  return output;
}

std::vector<Eigen::MatrixXf> GenerateRandomModelParams(int n) {
  srand(0);
  std::vector<Eigen::MatrixXf> output;
  for (int i = 0; i < n; ++i) {
    output.push_back(Eigen::MatrixXf::Random(2, 1));
  }
  return output;
}

class RansacTest : public ::testing::Test,
                   public ::testing::WithParamInterface<Eigen::MatrixXf> {
 public:
  void SetUp() {
    srand(0);
    opts = RansacOptions{
        .num_iterations = 1000,
        .inlier_threshold = 0.001,
        .min_inlier_fraction = 0.5,
        .random_seed = 0,
    };
    CHECK_GT(kNumInliers / static_cast<double>(kNumInliers + kNumOutliers),
             opts.min_inlier_fraction)
        << "The *actual* inlier ratio is smaller than the inlier ratio "
           "required for RANSAC to find the solution. Your test is specifying "
           "a scenario in which an algorithm will never succeed!!";
  }
  RansacOptions opts;
  static constexpr int kNumInliers = 10;
  static constexpr int kNumOutliers = 3;
};

INSTANTIATE_TEST_CASE_P(
    RansacTestSuite, RansacTest,
    ::testing::ValuesIn(GenerateRandomModelParams(FLAGS_num_test_cases)));

TEST_P(RansacTest, LeastSquareFitWithoutOutliersAndWithoutNoise) {
  // Verify that RANSAC can be used to fit a line in absence of noise and
  // absence of outliers.
  const Eigen::MatrixXf params = GetParam();
  std::vector<Eigen::MatrixXf> observations =
      CreateObservationCollection(params, /*num_observations=*/kNumInliers);
  auto solution =
      RansacFit(std::make_unique<LinearModel>(), observations, opts);
  ASSERT_TRUE(solution.success);
  EXPECT_THAT(solution.inliers, ::testing::Each(true));
  EXPECT_EQ(solution.num_inliers, observations.size());
  EXPECT_LE(solution.error, 1e-3);
  EXPECT_LE((solution.params - params).norm(), 1e-2);
}

TEST_P(RansacTest, LeastSquareFitWithOutliersAndWithoutNoise) {
  // Verify that RANSAC can be used to fit a line in absence of noise but with
  // some outliers.
  const Eigen::MatrixXf params = GetParam();
  std::vector<Eigen::MatrixXf> observations =
      CreateObservationCollection(params, /*num_observations=*/kNumInliers);
  std::vector<bool> inliers(kNumInliers, true);
  // Add several outliers.
  for (int i = 0; i < kNumOutliers; ++i) {
    observations.push_back(10.0 * Eigen::MatrixXf::Random(2, 1));
    inliers.push_back(false);
  }

  auto solution =
      RansacFit(std::make_unique<LinearModel>(), observations, opts);
  ASSERT_TRUE(solution.success);
  EXPECT_EQ(solution.num_inliers, kNumInliers);
  EXPECT_EQ(solution.inliers, inliers);
  EXPECT_LE(solution.error, 1e-3);
  EXPECT_LE((solution.params - params).norm(), 1e-2);
}

TEST(RansacRunOneIteration, Works) {
  LinearModel model;
  std::vector<Eigen::MatrixXf> observations{
      Eigen::MatrixXf(Eigen::Vector2f(0, 0)),
      Eigen::MatrixXf(Eigen::Vector2f(1, 1)),
      Eigen::MatrixXf(Eigen::Vector2f(2, 2)),
  };
  RansacOptions opts{
      .num_iterations = 0,  // unused
      .inlier_threshold = 0.001,
      .min_inlier_fraction = 0.5,
      .random_seed = 0,  // unused
  };
  auto soln = internal::RansacRunOneIteration(
      observations, std::unordered_set<int>{0, 1}, opts, model);
  ASSERT_TRUE(soln.success);
  EXPECT_NEAR(soln.error, 0.0, 1e-2);
  EXPECT_EQ(soln.num_inliers, 3);
}

TEST(RansacIsSolutionBetter, Works) {
  RansacSolution a{.success = true, .error = 1.0};
  RansacSolution b{.success = true, .error = 10.0};
  ASSERT_TRUE(internal::IsSolutionBetter(a, b));
}

}  // namespace dungeon
