#include "base/thread_pool.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace dungeon {

TEST(ThreadPoolTest, SingleThreadedExecution) {
  // When we have a single thread, a ThreadPool is equivalent to a task queue.
  ThreadPool pool(1);
  std::vector<float> values;
  const std::vector<float> kValueSequence{1, 2, 3, 4};
  for (float val : kValueSequence) {
    pool.Post([val, &values]() { values.push_back(val); });
  }
  pool.Wait();
  EXPECT_EQ(values, kValueSequence)
      << "Expected that order of values in the sequence would be preserved.";
}

TEST(ThreadPoolTest, ThreadPoolWorks) {
  // Each job writes to a different memory location; verify that resulting
  // values are correct.
  std::vector<float> values(4, 0.0f);
  ThreadPool pool(values.size());
  for (int i = 0; i < values.size(); ++i) {
    pool.Post([i, &values]() { values[i] += (i + 1); });
  }
  pool.Wait();
  EXPECT_EQ(values, (std::vector<float>{1, 2, 3, 4}));
}

}  // namespace dungeon
