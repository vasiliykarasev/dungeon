#include "math/sampling.h"

#include <random>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace dungeon {

TEST(UniformSubset, EdgeCases) {
  std::default_random_engine rng(0);
  EXPECT_TRUE(UniformSubset(rng, 0, 2).empty());
  EXPECT_TRUE(UniformSubset(rng, 0, 0).empty());
  EXPECT_EQ(std::unordered_set<int>{0}, UniformSubset(rng, 1, 1));
  EXPECT_EQ((std::unordered_set<int>{0, 1, 2}), UniformSubset(rng, 3, 3));
}

namespace {

// Samples subsets multiple times and returns fraction of times that each item
// was picked.
std::vector<double> EmpiricalSampleDistribution(std::default_random_engine& rng,
                                                int k, int n,
                                                int num_iterations) {
  std::vector<double> probs(n, 0.0);
  for (int i = 0; i < num_iterations; ++i) {
    for (int sample : UniformSubset(rng, 2, n)) {
      probs[sample] += 1.0 / num_iterations;
    }
  }
  return probs;
}

}  // namespace

TEST(UniformSubset, BasicCases) {
  using ::testing::DoubleNear;
  using ::testing::Each;
  // Verify that the distribution over samples is indeed uniform.
  std::default_random_engine rng(0);
  constexpr int kSubsetSize = 2;
  constexpr int kTotalSize = 10;
  constexpr int kNumIterations = 10000;
  std::vector<double> probs =
      EmpiricalSampleDistribution(rng, kSubsetSize, kTotalSize, kNumIterations);
  constexpr double kExpectedProb =
      kSubsetSize / static_cast<double>(kTotalSize);
  EXPECT_THAT(probs, Each(DoubleNear(kExpectedProb, 1e-2)));
}

}  // namespace dungeon
