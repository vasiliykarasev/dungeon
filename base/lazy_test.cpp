#include "base/lazy.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace dungeon {

TEST(LazyTest, MemoizationWorks) {
  int num_times_called = 0;
  Lazy<int> lazy_zero([&num_times_called]() {
    num_times_called += 1;
    return 0;
  });
  EXPECT_EQ(0, num_times_called)
      << "Should defer execution to the first call to Lazy<T>::get()";
  EXPECT_EQ(0, lazy_zero.get()) << "Should actually evaluate things here.";
  EXPECT_EQ(0, lazy_zero.get()) << "Should return the same result here.";
  EXPECT_EQ(1, num_times_called) << "Should have called the lambda only once.";
}

}  // namespace dungeon
