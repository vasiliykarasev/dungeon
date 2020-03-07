#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace dungeon {

// A wrapper for scheduling async tasks.
// Example:
//   {
//     TaskQueue queue;
//     queue.Post([](){ printf("hello\n"); });
//     queue.Post([](){ printf("goodbye\n"); });
//   }
//   >>> "hello";
//   >>> "goodbye";
class TaskQueue {
 public:
  TaskQueue();
  ~TaskQueue();

  // It is safe to call Post() from multiple threads, but you may not call it
  // while one of the threads is calling the dtor.
  void Post(std::function<void()> task);

 private:
  void WorkerLoop();

  std::queue<std::function<void()>> tasks_;
  std::thread thread_;
  std::mutex mu_;
  std::condition_variable cv_;
  bool is_stopping_{false};
};

}  // namespace dungeon
