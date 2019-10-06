#pragma once

#include <algorithm>
#include <deque>
#include <string>
#include <unordered_map>

namespace dungeon {

// Simple implementation of a least-recently-used cache.
// Insertions take O(1). Lookup is O(1) if the item is not in the cache,
// and O(n) if it does.
//
// TODO: this can be implemented with lookup and insertion in O(1) time if
// one uses list<std::pair<string, T>> to key/value pairs and a unordered_map
// to store key -> list iterator mapping.
template <typename T>
class LruCache {
 public:
  LruCache(size_t capacity) : capacity_(capacity) {}
  // Adds or replaces value for |key|.
  void Add(const std::string& key, T item);
  // Returns value for |key| if it exists, or a nullptr.
  const T* Get(const std::string& key);

 private:
  // Marks item as most recently used.
  void Touch(const std::string& key);
  void MaybeEvictLeastRecentlyUsed();

  size_t capacity_;
  std::unordered_map<std::string, T> items_;
  std::deque<std::string> usage_;
};

template <typename T>
void LruCache<T>::Add(const std::string& key, T item) {
  items_[key] = std::move(item);
  Touch(key);
  MaybeEvictLeastRecentlyUsed();
}

template <typename T>
const T* LruCache<T>::Get(const std::string& key) {
  auto it = items_.find(key);
  if (it == items_.end()) {
    return nullptr;
  }
  Touch(key);
  return &it->second;
}

template <typename T>
void LruCache<T>::Touch(const std::string& key) {
  auto it = std::find(usage_.begin(), usage_.end(), key);
  if (it != usage_.end()) {
    usage_.erase(it);
  }
  usage_.push_front(key);
}

template <typename T>
void LruCache<T>::MaybeEvictLeastRecentlyUsed() {
  while (items_.size() > capacity_) {
    const std::string& lru_key = usage_.back();
    items_.erase(lru_key);
    usage_.pop_back();
  }
}

}  // namespace dungeon
