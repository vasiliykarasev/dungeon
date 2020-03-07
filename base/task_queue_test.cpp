#include "base/task_queue.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace dungeon {

void IncrementReference(int& i) {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  ++i;
}

TEST(TaskQueueTest, Works) {
  //
  int val = 0;
  {  // Extra scope acts as a 'barrier': we wait for all tasks to finish prior
     // to checking the value of |val|.
    TaskQueue queue;
    for (int i = 0; i < 10; ++i) {
      queue.Post([&val]() { IncrementReference(val); });
    }
  }
  EXPECT_EQ(10, val);
}

}  // namespace dungeon
