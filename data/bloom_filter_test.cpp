#include <glog/logging.h>
#include <gtest/gtest.h>
#include <algorithm>
#include <unordered_set>

#include "data/bloom_filter.h"

namespace dungeon {

bool AllBitsSet(const std::vector<uint8_t>& bitset) {
  return std::all_of(bitset.begin(), bitset.end(),
                     [](uint8_t x) { return x == 255; });
}

bool NoBitsSet(const std::vector<uint8_t>& bitset) {
  return std::all_of(bitset.begin(), bitset.end(),
                     [](uint8_t x) { return x == 0; });
}

TEST(BloomFilter, Okay) {
  BloomFilter filter(/*num_hash_functions=*/1, /*bit_length=*/8);
  ASSERT_TRUE(NoBitsSet(filter.bitset()));
  for (const auto& item : {"car", "dog"}) {
    EXPECT_FALSE(filter.Contains(item)) << "Shouldn't contain '" << item << "'";
    filter.Add(item);
    EXPECT_TRUE(filter.Contains(item));
  }
}

TEST(BloomFilter, PerpetualFalsePositives) {
  BloomFilter filter(/*num_hash_functions=*/4, /*bit_length=*/64);
  ASSERT_TRUE(NoBitsSet(filter.bitset()));
  std::unordered_set<std::string> previous_keys;
  while (!AllBitsSet(filter.bitset())) {
    std::string key = std::to_string(rand());
    previous_keys.insert(key);
    filter.Add(key);
  }
  VLOG(1) << "Completely filled after " << previous_keys.size() << " keys.";
  // Now will always return true, regardless of whether the item was seen.
  for (int i = 0; i < 100; ++i) {
    ASSERT_TRUE(filter.Contains(std::to_string(rand())));
  }
}

}  // namespace dungeon
