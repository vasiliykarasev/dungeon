#pragma once
#include <utility>
#include <vector>

#include "Eigen/Dense"

namespace dungeon {

// Solves a stable-marriage / matching problem for the special case where
// maching costs are symmetric (preference of A being matched to B is the
// same as preference of B being matched to A), and can be represented by a
// single cost matrix.
// Input matrix does not need to be square, but values must be finite.
// Returns a collection of matches consisting of (row, col) indices into the
// cost matrix (i.e. cost can be looked up as:
// for (const std::pair<int, int>& pr : matches) {
//   LOG(INFO) << cost(std::get<0>(pr), std::get<1>(pr))
// }
std::vector<std::pair<size_t, size_t>> SolveGaleShapleyMatching(
    const Eigen::MatrixXf& cost);

}  // namespace dungeon
