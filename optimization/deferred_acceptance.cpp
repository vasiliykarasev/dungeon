#include "optimization/deferred_acceptance.h"

#include "glog/logging.h"
#include "glog/stl_logging.h"
#include "optimization/deferred_acceptance_impl.h"

namespace dungeon {

std::vector<std::pair<size_t, size_t>> SolveGaleShapleyMatching(
    const Eigen::MatrixXf& cost) {
  std::vector<internal::Proposer> men(cost.rows());
  std::vector<internal::Proposee> women(cost.cols());

  for (int i = 0; i < std::max(cost.rows(), cost.cols()); ++i) {
    bool should_continue =
        internal::SolveGaleShapleySingleIteration(cost, men, women);
    if (!should_continue) {
      break;
    }
  }

  std::vector<std::pair<size_t, size_t>> output;
  for (size_t i = 0; i < men.size(); ++i) {
    if (!men[i].IsEngaged()) {
      continue;
    }
    output.push_back(std::make_pair(i, men[i].candidate));
  }
  return output;
}

}  // namespace dungeon
