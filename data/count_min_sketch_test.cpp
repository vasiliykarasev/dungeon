#include <glog/logging.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <map>
#include <unordered_set>

#include "data/count_min_sketch.h"

namespace dungeon {

std::function<bool(size_t)> nonzero() {
  return [](size_t x) { return x != 0; };
}

TEST(CountMinSketch, TestSimple) {
  CountMinSketch sketch(/*num_hash_functions=*/1, /*table_size=*/4);
  for (int iter = 0; iter < 10; ++iter) {
    const std::string key{"0"};
    ASSERT_EQ(iter, sketch.Count(key));
    sketch.Add(key);
    EXPECT_EQ(1, std::count_if(sketch.counts()[0].begin(),
                               sketch.counts()[0].end(), nonzero()));
  }
}

TEST(CountMinSketch, TestOverCounting) {
  // We add many more elements than the number of bits we use for sketching.
  // Returned values overestimate the actual values.
  CountMinSketch sketch(/*num_hash_functions=*/1, /*table_size=*/4);
  for (int iter = 0; iter < 128; ++iter) {
    const std::string key = std::to_string(iter);
    const auto& counts = sketch.counts();
    ASSERT_EQ(1, counts.size());
    if (!std::all_of(counts[0].begin(), counts[0].end(), nonzero())) {
      sketch.Add(key);
    }
    ASSERT_LE(1, sketch.Count(key));
  }
}

namespace {

std::vector<size_t> add(const std::vector<size_t>& x,
                        const std::vector<size_t>& y) {
  CHECK_EQ(x.size(), y.size());
  std::vector<size_t> output(x.size(), 0);
  for (size_t i = 0; i < output.size(); ++i) {
    output[i] = x[i] + y[i];
  }
  return output;
}
std::vector<std::vector<size_t>> add(
    const std::vector<std::vector<size_t>>& xx,
    const std::vector<std::vector<size_t>>& yy) {
  std::vector<std::vector<size_t>> output(xx.size());
  CHECK_EQ(xx.size(), yy.size());
  for (size_t i = 0; i < output.size(); ++i) {
    output[i] = add(xx[i], yy[i]);
  }
  return output;
}

}  // namespace

TEST(CountMinSketch, TestMultipleHashFunctions) {
  constexpr int kNumAdds = 2;
  std::map<std::string, std::unique_ptr<CountMinSketch>> sketch_by_key;
  CountMinSketch sketch(4, 2);
  for (const std::string key : {"Alice", "Bob"}) {
    sketch_by_key[key] = std::make_unique<CountMinSketch>(
        /*num_hash_functions=*/4, /*table_size=*/2);
    for (int i = 0; i < kNumAdds; ++i) {
      sketch_by_key[key]->Add(key);
      sketch.Add(key);
    }
  }
  EXPECT_EQ(sketch.counts(), add(sketch_by_key["Alice"]->counts(),
                                 sketch_by_key["Bob"]->counts()));
  EXPECT_EQ(kNumAdds, sketch.Count("Alice"));
  EXPECT_EQ(kNumAdds, sketch.Count("Bob"));
}

}  // namespace dungeon
