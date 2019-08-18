#pragma once
#include <string>
#include <vector>

namespace dungeon {

// A sketching data structure to approximate whether an item has been seen.
// Has a false-positive rate proportional to:
//     alpha = exp(-num_hash_functions/bit_length*num_items_inserted)
//     prob_fp = (1 - alpha)^num_hash_functions
class BloomFilter {
 public:
  BloomFilter(size_t num_hash_functions, size_t bit_length);

  void Add(const std::string& key);
  bool Contains(const std::string& key) const;

  // Exposed for testing.
  const std::vector<uint8_t>& bitset() const;

 private:
  size_t num_hash_functions_;
  size_t bit_length_;
  std::vector<uint8_t> bitset_;
};

}  // namespace dungeon
