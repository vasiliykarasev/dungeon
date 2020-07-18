#pragma once

#include <unordered_set>

#include "optimization/ransac.h"

namespace dungeon {
namespace internal {

// Performs a single iteration of Ransac algorithm.
RansacSolution RansacRunOneIteration(
    const std::vector<Eigen::MatrixXf>& observations,
    const std::unordered_set<int>& candidate_inliers, const RansacOptions& opt,
    RansacModelBase& model);

// Returns true if solution `a` is better than solution `b`.
bool IsSolutionBetter(const RansacSolution& a, const RansacSolution& b);

}  // namespace internal
}  // namespace dungeon
