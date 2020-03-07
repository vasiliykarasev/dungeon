#include "base/periodic_task.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace dungeon {

void IncrementReference(int& i) { ++i; }

TEST(TaskQueueTest, IncrementsVariableManyTimes) {
  // Create a periodic task, assign it to periodically increment a variable,
  // and make sure that the variable gets increment at least some number of
  // times.
  int val = 0;
  {
    PeriodicTask task([&val]() { IncrementReference(val); },
                      std::chrono::nanoseconds(1));
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  EXPECT_LE(100, val);
}

TEST(TaskQueueTest, TaskIsCalledImmediatelyUponCreation) {
  // Task is called immediately after PeriodicTask is created (in other words,
  // we do not need to wait |interval| until it is called for the first time).
  bool was_called = false;
  {  // Scope is used as a barrier, to ensure that the object is destroyed
     // before we check the value of |was_called|.
    PeriodicTask task([&was_called] { was_called = true; },
                      std::chrono::seconds(1));
  }
  ASSERT_TRUE(was_called);
}

}  // namespace dungeon
