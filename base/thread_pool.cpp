#include "base/thread_pool.h"

#include "glog/logging.h"

namespace dungeon {

ThreadPool::ThreadPool(int num_threads) {
  CHECK_GE(num_threads, 0);
  for (int i = 0; i < num_threads; ++i) {
    threads_.push_back(std::thread(&ThreadPool::WorkerFunc, this));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::lock_guard<std::mutex> lock(mu_);
    // Post N 'null' tasks that signify a 'stop' condition.
    // This trick is also used in abseil. Another possibility is to use a
    // 'stopped' boolean flag and checking for it in WorkerFunc().
    for (size_t i = 0; i < threads_.size(); ++i) {
      tasks_.push(nullptr);
    }
  }
  // Ensure that all threads process newly posted tasks.
  cv_.notify_all();
  for (auto& t : threads_) {
    t.join();
  }
}

void ThreadPool::Post(std::function<void()> task) {
  {
    std::lock_guard<std::mutex> lock(mu_);
    tasks_.push(std::move(task));
  }
  cv_.notify_one();
}

void ThreadPool::Wait() {
  std::unique_lock<std::mutex> lock(mu_);
  if (tasks_.empty()) {
    return;
  }
  cv_.wait(lock, [this]() { return tasks_.empty(); });
}

void ThreadPool::WorkerFunc() {
  while (true) {
    std::function<void()> task = nullptr;
    {
      std::unique_lock<std::mutex> lock(mu_);
      cv_.wait(lock, [this] { return !tasks_.empty(); });

      CHECK(!tasks_.empty());
      task = tasks_.front();
      tasks_.pop();
    }
    // User isnt allowed to post null tasks, so it must be the shutdown signal.
    if (task == nullptr) {
      return;
    }
    task();

    // Post notification: we may be waiting for the task queue to become empty.
    // Note: some implementations may use a separate condition variable for
    // this (to be used here, and in ThreadPool::Wait()).
    cv_.notify_all();
  }
}

}  // namespace dungeon
