#include <functional>
#include <mutex>
#include <thread>

#include "glog/logging.h"

namespace dungeon {

template <typename T>
class Lazy {
 public:
  explicit Lazy(std::function<T()> func) : func_(std::move(func)) {}

  // Evaluates wrapped function once, and subsequently just returns the memoized
  // result.
  T get() {
    std::call_once(flag_, [this]() {
      CHECK(func_);
      this->result_ = func_();
      // Clear the function, since we will never call it again.
      func_ = nullptr;
    });
    return result_;
  }

 private:
  T result_;
  std::once_flag flag_;
  std::function<T(void)> func_;
};

}  // namespace dungeon
