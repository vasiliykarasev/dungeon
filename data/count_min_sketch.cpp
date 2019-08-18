#include "data/count_min_sketch.h"

#include <glog/logging.h>

namespace dungeon {

CountMinSketch::CountMinSketch(size_t num_hash_functions, size_t table_size)
    : num_hash_functions_(num_hash_functions), table_size_(table_size) {
  CHECK_GT(num_hash_functions_, 0);
  CHECK_GT(table_size_, 0);
  counts_ = std::vector<std::vector<size_t>>(
      num_hash_functions_, std::vector<size_t>(table_size_, 0));
}

void CountMinSketch::Add(const std::string& key) {
  std::hash<std::string> hasher;
  for (size_t i = 0; i < num_hash_functions_; ++i) {
    size_t digest = hasher(key + std::to_string(i)) % table_size_;
    counts_[i][digest]++;
  }
}

size_t CountMinSketch::Count(const std::string& key) const {
  std::hash<std::string> hasher;

  size_t output = std::numeric_limits<size_t>::max();
  for (size_t i = 0; i < num_hash_functions_; ++i) {
    size_t digest = hasher(key + std::to_string(i)) % table_size_;
    output = std::min(output, counts_[i][digest]);
  }
  return output;
}

const std::vector<std::vector<size_t>>& CountMinSketch::counts() const {
  return counts_;
}

}  // namespace dungeon
