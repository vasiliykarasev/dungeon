#pragma once
#include <string>
#include <vector>

namespace dungeon {

class BloomFilter {
 public:
  BloomFilter(size_t num_hash_functions, size_t bit_length);

  void Add(const std::string& key);
  bool Contains(const std::string& key) const;

  const std::vector<uint8_t>& bitset() const;

 private:
  size_t num_hash_functions_;
  size_t bit_length_;
  std::vector<uint8_t> bitset_;
};

}  // namespace dungeon
