#include "base/periodic_task.h"

#include "glog/logging.h"

namespace dungeon {

PeriodicTask::PeriodicTask(std::function<void()> task,
                           std::chrono::duration<int64_t, std::nano> interval)
    : thread_([this, &task] { WorkerLoop(std::move(task)); }),
      interval_(interval),
      is_stopping_(false) {}

PeriodicTask::~PeriodicTask() {
  {
    std::unique_lock<std::mutex> lock(mu_);
    is_stopping_ = true;
  }
  cv_.notify_all();
  thread_.join();
}

void PeriodicTask::WorkerLoop(std::function<void()> task) {
  while (true) {
    task();
    {
      std::unique_lock<std::mutex> lock(mu_);
      cv_.wait_for(lock, interval_, [this] { return is_stopping_; });
      if (is_stopping_) {
        return;
      }
    }
  }
}

}  // namespace dungeon
