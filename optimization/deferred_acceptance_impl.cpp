#include "optimization/deferred_acceptance_impl.h"

#include "glog/logging.h"
#include "glog/stl_logging.h"

namespace dungeon {
namespace internal {

bool SolveGaleShapleySingleIteration(const Eigen::MatrixXf& cost,
                                     std::vector<Proposer>& men,
                                     std::vector<Proposee>& women) {
  CHECK_EQ(men.size(), cost.rows());
  CHECK_EQ(women.size(), cost.cols());

  // Find a list of pairs, sorted by matching cost (which is symmetric).
  std::multimap<float, std::pair<int, int>> candidate_matches =
      ProposeCandidateMatches(men, cost);
  // If we didn't find any possible matching candidates, performing another
  // iteration will not help: can stop now.
  if (candidate_matches.empty()) {
    return false;
  }
  for (const auto& pr : candidate_matches) {
    float cost = pr.first;
    int man_idx = std::get<0>(pr.second);
    int woman_idx = std::get<1>(pr.second);

    Proposee& woman = women[woman_idx];
    Proposer& man = men[man_idx];

    man.previously_proposed_to.insert(woman_idx);
    CHECK(!man.IsEngaged());
    if (!woman.IsEngaged()) {
      man.candidate = woman_idx;
      woman.candidate = man_idx;
      woman.matching_cost = cost;
    } else if (woman.IsEngaged() && woman.matching_cost > cost) {
      int prev_suitor_idx = woman.candidate;
      men[prev_suitor_idx].candidate = -1;
      woman.candidate = man_idx;
      woman.matching_cost = cost;
      man.candidate = woman_idx;
    }
  }
  return true;
}

int Proposer::BestCandidate(const Eigen::VectorXf& cost) const {
  if (static_cast<int>(previously_proposed_to.size()) == cost.size()) {
    return -1;
  }
  int min_idx = -1;
  float min_cost = std::numeric_limits<float>::infinity();
  for (int i = 0; i < cost.size(); ++i) {
    if (previously_proposed_to.count(i)) {
      continue;
    }
    if (cost(i) < min_cost) {
      min_cost = cost(i);
      min_idx = i;
    }
  }
  return min_idx;
}

std::multimap<float, std::pair<int, int>> ProposeCandidateMatches(
    const std::vector<Proposer>& proposers, const Eigen::MatrixXf& cost) {
  CHECK_EQ(proposers.size(), cost.rows());
  std::multimap<float, std::pair<int, int>> output;
  for (size_t i = 0; i < proposers.size(); ++i) {
    const auto& proposer = proposers[i];
    if (proposer.IsEngaged()) {
      continue;
    }
    int j = proposer.BestCandidate(cost.row(i));
    // Couldn't find a candidate (already proposed to everyone).
    if (j < 0) {
      continue;
    }
    CHECK_LT(j, cost.cols());
    output.insert(std::make_pair(cost(i, j), std::make_pair(i, j)));
  }
  return output;
}

bool IsPairUnstable(const Eigen::MatrixXf& _cost,
                    const std::pair<size_t, size_t>& m1,
                    const std::pair<size_t, size_t>& m2) {
  // A couple of pairs with swapped partners.
  auto m1_alt = std::make_pair(std::get<0>(m1), std::get<1>(m2));
  auto m2_alt = std::make_pair(std::get<0>(m2), std::get<1>(m1));
  // Helper to make expressions more readable.
  auto cost = [&_cost](const std::pair<int, int>& m) {
    return _cost(std::get<0>(m), std::get<1>(m));
  };
  return cost(m1) > cost(m1_alt) && cost(m2) > cost(m2_alt);
}

bool IsMatchingUnstable(const Eigen::MatrixXf& cost,
                        const std::vector<std::pair<size_t, size_t>>& matches) {
  for (size_t i = 0; i < matches.size(); ++i) {
    for (size_t j = i + 1; j < matches.size(); ++j) {
      if (IsPairUnstable(cost, matches[i], matches[j])) {
        return true;
      }
    }
  }
  return false;
}

}  // namespace internal
}  // namespace dungeon
