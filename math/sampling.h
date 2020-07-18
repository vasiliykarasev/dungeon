#include <random>
#include <unordered_set>

#include "glog/logging.h"

namespace dungeon {

// Returns a vector of k (unique) elements sampled uniformly from {0, ..., n-1},
// using k calls to the random number generator.
template <typename Generator>
std::unordered_set<int> UniformSubset(Generator& rng, int k, int n) {
  CHECK_LE(0, n);
  CHECK_LE(k, n);
  std::unordered_set<int> output;
  for (int j = n - k; j < n; ++j) {
    int sample = rng() % (j + 1);  // in {0, ..., j}
    if (!output.count(sample)) {
      output.insert(sample);
    } else {
      output.insert(j);
    }
  }
  return output;
}

}  // namespace dungeon
