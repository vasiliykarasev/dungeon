#include "optimization/ransac_impl.h"

#include "glog/logging.h"

namespace dungeon {
namespace internal {

bool IsSolutionBetter(const RansacSolution& a, const RansacSolution& b) {
  if (a.success != b.success) {
    return a.success > b.success;
  }
  return a.error <= b.error;
}

RansacSolution RansacRunOneIteration(
    const std::vector<Eigen::MatrixXf>& observations,
    const std::unordered_set<int>& candidate_inliers, const RansacOptions& opt,
    RansacModelBase& model) {
  CHECK_GE(candidate_inliers.size(), model.min_num_observations());
  CHECK_GE(observations.size(), model.min_num_observations());

  std::vector<Eigen::MatrixXf> candidate_pts;
  candidate_pts.reserve(model.min_num_observations());
  for (auto idx : candidate_inliers) {
    candidate_pts.push_back(observations[idx]);
  }
  model.Fit(candidate_pts);

  RansacSolution output{
      .success = false,
      .params = model.params(),
      .num_inliers = 0,
      .error = 0,
      .inliers = std::vector<bool>(observations.size(), false),
  };
  for (size_t i = 0; i < observations.size(); ++i) {
    double obs_error = model.EvaluateError(observations[i]);
    bool is_inlier = candidate_inliers.count(i) ||
                     static_cast<bool>(obs_error <= opt.inlier_threshold);
    if (is_inlier) {
      output.error += obs_error;
      output.inliers[i] = true;
      output.num_inliers += 1;
    }
  }
  output.success =
      output.num_inliers >= observations.size() * opt.min_inlier_fraction;
  return output;
}

}  // namespace internal
}  // namespace dungeon
