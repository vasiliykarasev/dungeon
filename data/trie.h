#pragma once

#include <functional>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <vector>

namespace dungeon {

template <typename T>
class Trie {
 public:
  void Add(const std::vector<uint8_t>& key, T item);
  bool Contains(const std::vector<uint8_t>& key) const;
  size_t Size() const;
  bool empty() const;

  // Returns the set of items matching the given prefix.
  std::set<T> FindForKeyPrefix(const std::vector<uint8_t>& key_prefix);

 private:
  // Returns the node for a given key.
  const Trie<T>* FindOrNull(const std::vector<uint8_t>& key) const;
  // Applies the callback to the root node and all descendents.
  void Visit(std::function<void(const Trie<T>&)> cb) const;

  std::optional<T> item_;
  std::map<uint8_t, Trie<T>> children_;
};

template <typename T>
void Trie<T>::Add(const std::vector<uint8_t>& key, T item) {
  Trie<T>* node = this;
  for (const uint8_t x : key) {
    node = &(node->children_[x]);
  }
  node->item_ = std::move(item);
}

template <typename T>
const Trie<T>* Trie<T>::FindOrNull(const std::vector<uint8_t>& key) const {
  const Trie<T>* node = this;
  for (const uint8_t x : key) {
    auto it = node->children_.find(x);
    if (it == node->children_.end()) {
      return nullptr;
    } else {
      node = &it->second;
    }
  }
  return node;
}

template <typename T>
bool Trie<T>::Contains(const std::vector<uint8_t>& key) const {
  const auto* node = FindOrNull(key);
  return node && node->item_.has_value();
}

template <typename T>
bool Trie<T>::empty() const {
  return children_.empty() && !item_.has_value();
}

template <typename T>
void Trie<T>::Visit(std::function<void(const Trie<T>&)> visit_cb) const {
  std::queue<const Trie<T>*> queue;
  queue.push(this);
  while (!queue.empty()) {
    const Trie<T>* current = queue.front();
    visit_cb(*current);
    queue.pop();

    for (const auto& pr : current->children_) {
      queue.push(&pr.second);
    }
  }
}

template <typename T>
size_t Trie<T>::Size() const {
  size_t size = 0;
  Visit([&size](const Trie<T>& node) { size += node.item_.has_value(); });
  return size;
}

template <typename T>
std::set<T> Trie<T>::FindForKeyPrefix(const std::vector<uint8_t>& key_prefix) {
  const auto* node = FindOrNull(key_prefix);
  if (!node) {
    return {};
  }
  std::set<T> output;
  node->Visit([&output](const Trie<T>& node) {
    if (node.item_.has_value()) {
      output.insert(*node.item_);
    }
  });
  return output;
}

}  // namespace dungeon
