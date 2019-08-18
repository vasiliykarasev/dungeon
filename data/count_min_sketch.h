#pragma once
#include <string>
#include <vector>

namespace dungeon {

// Sketching data structure used to approximates the number of times an element
// has been seen, using |num_hash_functions|*|table_size|*sizeof(size_t) bytes.
class CountMinSketch {
 public:
  CountMinSketch(size_t num_hash_functions, size_t table_size);

  // Adds an element to the history.
  void Add(const std::string& key);

  // Returns the number of times an element has been seen. Returned value
  // may exceed the actual number of times an element has been seen.
  size_t Count(const std::string& key) const;

  // Exposed for testing.
  const std::vector<std::vector<size_t>>& counts() const;

 private:
  size_t num_hash_functions_;
  size_t table_size_;
  std::vector<std::vector<size_t>> counts_;
};

}  // namespace dungeon
