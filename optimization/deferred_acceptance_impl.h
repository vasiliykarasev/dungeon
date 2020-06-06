#pragma once
#include <map>
#include <unordered_set>

#include "Eigen/Dense"

// Contains internal helper structures used in solving Gale-Shapley matching.
namespace dungeon {
namespace internal {

struct Proposer {
  bool IsEngaged() const { return candidate >= 0; }
  // Returns argmin in cost that's not in previously_proposed_to.
  // If cost.size() == previously_proposed_to.size(), returns -1.
  int BestCandidate(const Eigen::VectorXf& cost) const;

  // If non-negative, represents index of the proposee that the entity is
  // matched to.
  int candidate = -1;
  std::unordered_set<int> previously_proposed_to;
};

struct Proposee {
  bool IsEngaged() const { return candidate >= 0; }
  // If non-negative
  int candidate = -1;
  float matching_cost = std::numeric_limits<float>::infinity();
};

// Returns a priority queue from matching cost to (proposer, proposee) pairs.
std::multimap<float, std::pair<int, int>> ProposeCandidateMatches(
    const std::vector<Proposer>& proposers, const Eigen::MatrixXf& cost);

// Returns true if another iteration may be needed: specifically if we've
// found at least one candidate assignment.
bool SolveGaleShapleySingleIteration(const Eigen::MatrixXf& cost,
                                     std::vector<Proposer>& men,
                                     std::vector<Proposee>& women);

// Returns true if a pair of matches are better off 'swapping' partners.
// Can be used in testing / verification.
bool IsPairUnstable(const Eigen::MatrixXf& _cost,
                    const std::pair<size_t, size_t>& m1,
                    const std::pair<size_t, size_t>& m2);

// Returns true if any pair of matches is better off 'swapping' partners.
// Can be used in testing/verification.
bool IsMatchingUnstable(const Eigen::MatrixXf& cost,
                        const std::vector<std::pair<size_t, size_t>>& matches);
}  // namespace internal
}  // namespace dungeon
