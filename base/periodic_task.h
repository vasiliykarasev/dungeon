#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace dungeon {

// A wrapper for calling the same function again and again, at a fixed cadence.
class PeriodicTask {
 public:
  PeriodicTask(std::function<void()> task,
               std::chrono::duration<int64_t, std::nano> interval);
  ~PeriodicTask();

 private:
  void WorkerLoop(std::function<void()> task);

  std::thread thread_;
  std::mutex mu_;
  std::condition_variable cv_;
  std::chrono::duration<int64_t, std::nano> interval_;
  bool is_stopping_{false};
};

}  // namespace dungeon
