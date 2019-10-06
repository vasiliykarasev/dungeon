#include <gtest/gtest.h>

#include "data/lru_cache.h"

namespace dungeon {

TEST(LruCache, GetBasic) {
  LruCache<std::string> cache(/*capacity=*/1);
  {  // Item is not in the cache; returns a nullptr.
    const auto* item_from_cache = cache.Get("hello");
    ASSERT_FALSE(static_cast<bool>(item_from_cache));
  }
  cache.Add("hello", "hello");
  {  // Item is in the cache; returns it.
    const auto* item_from_cache = cache.Get("hello");
    ASSERT_TRUE(static_cast<bool>(item_from_cache));
    EXPECT_EQ("hello", *item_from_cache);
  }
}

#define EXPECT_ITEM_IN_CACHE_AND_EQ(expected, expr) \
  do {                                              \
    const auto* item = (expr);                      \
    ASSERT_TRUE(static_cast<bool>(item));           \
    EXPECT_EQ((expected), *item);                   \
  } while (0)

TEST(LruCache, EvictionOrderSimple) {
  // Keep adding new values, and verify that the |capacity|
  // entries remain in the cache (and that others are evicted).
  constexpr int kCapacity = 4;
  constexpr int kNumItemsAdded = 10;
  static_assert(kNumItemsAdded > kCapacity);
  LruCache<int> cache(kCapacity);
  for (int i = 0; i < kNumItemsAdded; ++i) {
    cache.Add(std::to_string(i), i);
    EXPECT_ITEM_IN_CACHE_AND_EQ(i, cache.Get(std::to_string(i)));
  }
  for (int i = 0; i < kNumItemsAdded - kCapacity; ++i) {
    EXPECT_FALSE(static_cast<bool>(cache.Get(std::to_string(i))));
  }
  for (int i = kNumItemsAdded - kCapacity; i < kNumItemsAdded; ++i) {
    EXPECT_ITEM_IN_CACHE_AND_EQ(i, cache.Get(std::to_string(i)));
  }
}

TEST(LruCache, PersistDespiteEviction) {
  // One of the items gets queried over and over again, and ends up
  // persisting in the cache.
  constexpr int kCapacity = 2;
  constexpr int kNumItemsAdded = 10;
  static_assert(kNumItemsAdded > kCapacity);
  LruCache<int> cache(kCapacity);
  for (int i = 0; i < kNumItemsAdded; ++i) {
    cache.Add(std::to_string(i), i);
    cache.Get(std::to_string(0));
  }

  for (int i = 0; i < kNumItemsAdded; ++i) {
    if (i == 0 || i == kNumItemsAdded - 1) {
      EXPECT_TRUE(static_cast<bool>(cache.Get(std::to_string(i))))
          << "Item '" << i << "' should have been in cache!";
    } else {
      EXPECT_FALSE(static_cast<bool>(cache.Get(std::to_string(i))))
          << "Item '" << i << "' should have been evicted!";
    }
  }
}

}  // namespace dungeon
