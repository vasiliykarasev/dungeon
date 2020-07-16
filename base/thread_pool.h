#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace dungeon {

// Simple thread pool inspired by absl and colmap.
class ThreadPool {
 public:
  ThreadPool(int num_threads);
  ~ThreadPool();

  void Post(std::function<void()> task);

  void Wait();

 private:
  void WorkerFunc();

  std::vector<std::thread> threads_;
  std::queue<std::function<void()>> tasks_;
  std::mutex mu_;
  std::condition_variable cv_;
};

}  // namespace dungeon
