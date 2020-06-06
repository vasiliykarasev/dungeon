#pragma once
#include <tuple>

namespace dungeon {
// Python-like syntax for enumerating a collection.
// Mostly stolen from:
//   http://reedbeta.com/blog/python-like-enumerate-in-cpp17/
// See enumerate_test for usage examples.
// This is primarily useful with C++17.
template <typename T,
          typename IteratorType = decltype(std::begin(std::declval<T>())),
          typename = decltype(std::end(std::declval<T>()))>
constexpr auto enumerate(T&& iterable) {
  struct EnumeratingIterator {
    size_t i;
    IteratorType iter;
    bool operator!=(const EnumeratingIterator& other) const {
      return iter != other.iter;
    }
    void operator++() {
      ++i;
      ++iter;
    }
    auto operator*() const { return std::tie(i, *iter); }
  };
  struct IterableWrapper {
    T iterable;
    auto begin() { return EnumeratingIterator{0, std::begin(iterable)}; }
    auto end() { return EnumeratingIterator{0, std::end(iterable)}; }
  };
  return IterableWrapper{std::forward<T>(iterable)};
}

}  // namespace dungeon
