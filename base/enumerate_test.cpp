#include "base/enumerate.h"

#include "gtest/gtest.h"

namespace dungeon {

TEST(Enumerate, RangeForLoop) {
  std::vector<int> container{0, 1, 2, 3};
  for (const auto& [idx, elem] : enumerate(std::move(container))) {
    EXPECT_EQ(idx, elem);
  }
  for (const auto& [idx, _] : enumerate(std::vector<int>{1})) {
    EXPECT_EQ(0, idx);
  }
}

TEST(Enumerate, OldFamiliarLoop) {
  std::vector<int> container{0, 1, 2};
  auto enumerating_iter = enumerate(container);
  for (auto it = enumerating_iter.begin(); it != enumerating_iter.end(); ++it) {
    EXPECT_EQ(std::get<0>(*it), std::get<1>(*it));
  }
}

}  // namespace dungeon
