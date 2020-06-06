#include "optimization/deferred_acceptance.h"

#include <algorithm>
#include <random>

#include "glog/logging.h"
#include "glog/stl_logging.h"
#include "gtest/gtest.h"
#include "optimization/deferred_acceptance_impl.h"

namespace dungeon {

TEST(GaleShapley, SolveSingleIterationWithIdentityCost) {
  std::vector<internal::Proposer> men(2);
  std::vector<internal::Proposee> women(2);
  Eigen::MatrixXf cost = Eigen::MatrixXf::Identity(2, 2);
  internal::SolveGaleShapleySingleIteration(cost, men, women);
  EXPECT_EQ(0.0f, women[0].matching_cost);
  EXPECT_EQ(0.0f, women[1].matching_cost);
  EXPECT_EQ(1, men[0].candidate);
  EXPECT_EQ(0, men[1].candidate);
  EXPECT_EQ(1, women[0].candidate);
  EXPECT_EQ(0, women[1].candidate);
}

TEST(GaleShapley, SolveSingleIteration) {
  std::vector<internal::Proposer> men(2);
  std::vector<internal::Proposee> women(2);
  Eigen::MatrixXf cost = Eigen::MatrixXf::Zero(2, 2);
  cost(0, 1) = 1.0;
  cost(1, 1) = 1.0;
  internal::SolveGaleShapleySingleIteration(cost, men, women);
  EXPECT_EQ(0, men[0].candidate);
  EXPECT_EQ(0, women[0].candidate);
  EXPECT_FALSE(men[1].IsEngaged());
  EXPECT_FALSE(women[1].IsEngaged());
}

TEST(IsMatchingUnstable, Works) {
  // Each pair is better off switching.
  EXPECT_TRUE(internal::IsMatchingUnstable(Eigen::MatrixXf::Identity(3, 3),
                                           {{0, 0}, {1, 1}, {2, 2}}));
  // Two pairs pay zero cost, and one pair pays the maximum cost, but no two
  // pairs benefit from switching.
  EXPECT_FALSE(internal::IsMatchingUnstable(Eigen::MatrixXf::Identity(3, 3),
                                            {{0, 1}, {1, 0}, {2, 2}}));
}

bool CostIsZero(const Eigen::MatrixXf& cost,
                const std::vector<std::pair<size_t, size_t>>& matches) {
  return std::all_of(matches.begin(), matches.end(),
                     [&cost](const std::pair<size_t, size_t>& ij) {
                       return cost(std::get<0>(ij), std::get<1>(ij)) == 0.0;
                     });
}

TEST(GaleShapley, TrivialIdentityCase) {
  // A simple situation where zero cost can be achieved and matching is
  // unique.
  Eigen::MatrixXf cost = Eigen::MatrixXf::Identity(2, 2);
  auto matches = SolveGaleShapleyMatching(cost);
  ASSERT_EQ(matches, (std::vector<std::pair<size_t, size_t>>{{0, 1}, {1, 0}}));
  EXPECT_TRUE(CostIsZero(cost, matches));
}

TEST(GaleShapley, SimpleNonsquareIdentityCases) {
  // A simple situation where zero cost be achieved, but not everyone can be
  // matched.
  for (const auto& cost : std::vector<Eigen::MatrixXf>{
           Eigen::MatrixXf::Identity(4, 2), Eigen::MatrixXf(2, 4)}) {
    auto matches = SolveGaleShapleyMatching(cost);
    ASSERT_EQ(2, matches.size());
    EXPECT_TRUE(CostIsZero(cost, matches));
  }
}

TEST(GaleShapley, RandomizedPropertyTest) {
  // Verify simple properties of the matching:
  // - that the number of matches is exactly min(rows, cols) in the cost.
  // - that nobody is matched more than once.
  // - that the matching is stable.
  // - Note: Gale-Shapley does *NOT* guarantee that the total matching cost
  //         is minimized.
  constexpr int kNumIterations = 100;
  constexpr int kRows = 3;
  constexpr int kCols = 6;
  std::default_random_engine rng(0);
  for (int i = 0; i < kNumIterations; ++i) {
    Eigen::MatrixXf cost = Eigen::MatrixXf::Random(kRows, kCols);
    auto matches = SolveGaleShapleyMatching(cost);
    ASSERT_EQ(matches.size(), std::min<size_t>(cost.rows(), cost.cols()));
    std::unordered_set<int> row_idxs;
    std::unordered_set<int> col_idxs;
    for (const auto& m : matches) {
      ASSERT_TRUE(!row_idxs.count(std::get<0>(m)))
          << "Row index appears most than once (a 'man' was matched to more "
             "than one 'woman')";
      ASSERT_TRUE(!col_idxs.count(std::get<1>(m)))
          << "Row index appears most than once (a 'man' was matched to more "
             "than one 'woman')";
      row_idxs.insert(std::get<0>(m));
      col_idxs.insert(std::get<1>(m));
    }
    ASSERT_FALSE(internal::IsMatchingUnstable(cost, matches))
        << "Gale-Shapley algorithm is expected to produce stable matchings.";
  }
}

}  // namespace dungeon
