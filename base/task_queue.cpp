#include "base/task_queue.h"

#include "glog/logging.h"

namespace dungeon {

TaskQueue::TaskQueue() : thread_([this] { WorkerLoop(); }) {}

TaskQueue::~TaskQueue() {
  {
    std::unique_lock<std::mutex> lock(mu_);
    is_stopping_ = true;
  }
  cv_.notify_all();
  thread_.join();
}

void TaskQueue::Post(std::function<void()> task) {
  {
    std::unique_lock<std::mutex> lock(mu_);
    // Discussion: another possibility would be to 'fail' posting a task (this
    // means that the signature of Post() needs to be different).
    CHECK(!is_stopping_)
        << "You are trying to destroy the task queue object, while "
           "simultaneously adding tasks to it. Likely, the two operations are "
           "happening on different threads. This is not allowed.";
    tasks_.push(task);
  }
  cv_.notify_all();
}

void TaskQueue::WorkerLoop() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(mu_);
      cv_.wait(lock, [this] { return is_stopping_ || !tasks_.empty(); });
      if (is_stopping_ && tasks_.empty()) {
        return;
      }
      task = std::move(tasks_.front());
      tasks_.pop();
    }
    task();
  }
}

}  // namespace dungeon
