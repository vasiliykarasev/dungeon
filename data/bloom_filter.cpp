#include "data/bloom_filter.h"

#include <glog/logging.h>

namespace dungeon {

BloomFilter::BloomFilter(size_t num_hash_functions, size_t bit_length)
    : num_hash_functions_(num_hash_functions), bit_length_(bit_length) {
  CHECK_EQ(0, bit_length % 8) << "Bit length must be divisible by 8";
  bitset_ = std::vector<uint8_t>(bit_length / 8, 0);
}

void BloomFilter::Add(const std::string& key) {
  std::hash<std::string> hasher;
  for (size_t i = 0; i < num_hash_functions_; ++i) {
    size_t digest = hasher(key + std::to_string(i)) % bit_length_;
    size_t idx = digest / 8;
    size_t bit_idx = digest % 8;
    bitset_[idx] |= (1 << bit_idx);
  }
}

bool BloomFilter::Contains(const std::string& key) const {
  std::hash<std::string> hasher;
  for (size_t i = 0; i < num_hash_functions_; ++i) {
    size_t digest = hasher(key + std::to_string(i)) % bit_length_;
    size_t idx = digest / 8;
    size_t bit_idx = digest % 8;
    if (!(bitset_[idx] & (1 << bit_idx))) {
      return false;
    }
  }
  return true;
}

const std::vector<uint8_t>& BloomFilter::bitset() const { return bitset_; }

}  // namespace dungeon
