#include "optimization/ransac.h"

#include "glog/logging.h"
#include "math/sampling.h"
#include "optimization/ransac_impl.h"

namespace dungeon {

RansacSolution RansacFit(std::unique_ptr<RansacModelBase> model,
                         const std::vector<Eigen::MatrixXf>& observations,
                         const RansacOptions& opt) {
  CHECK(model);
  CHECK(!observations.empty());
  // Create a 'dummy' solution.
  RansacSolution soln{
      .success = false,
      .params = model->params(),
      .num_inliers = 0,
      .error = std::numeric_limits<double>::max(),
      .inliers = std::vector<bool>(observations.size(), false),
  };
  std::default_random_engine rng(opt.random_seed);
  for (int i = 0; i < opt.num_iterations; ++i) {
    std::unordered_set<int> candidate_inliers =
        UniformSubset(rng, model->min_num_observations(), observations.size());
    RansacSolution candidate_soln = internal::RansacRunOneIteration(
        observations, candidate_inliers, opt, *model);
    if (internal::IsSolutionBetter(candidate_soln, soln)) {
      soln = std::move(candidate_soln);
    }
  }
  return soln;
}

}  // namespace dungeon
