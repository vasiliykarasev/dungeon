#include "data/trie.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

namespace dungeon {

TEST(Trie, SizeWorks) {
  Trie<int> trie;
  ASSERT_EQ(0, trie.Size());
  trie.Add({1, 2}, 12);
  trie.Add({1}, 1);
  ASSERT_EQ(2, trie.Size());
  trie.Add({3}, 3);
  ASSERT_EQ(3, trie.Size());
  trie.Add({3, 4}, 34);
  trie.Add({3, 4, 5}, 345);
  ASSERT_EQ(5, trie.Size());
  // Add already existing elements: size should not change.
  trie.Add({3, 4, 5}, 345);
  ASSERT_EQ(5, trie.Size());
  trie.Add({1}, 1);
  ASSERT_EQ(5, trie.Size());
}

TEST(Trie, ContainsWorks) {
  Trie<int> trie;
  ASSERT_FALSE(trie.Contains({1, 2, 3}));
  ASSERT_TRUE(trie.empty());
  trie.Add({1, 2, 3}, 123);
  EXPECT_FALSE(trie.Contains({1}));
  EXPECT_FALSE(trie.Contains({1, 2}));
  ASSERT_TRUE(trie.Contains({1, 2, 3}));
  ASSERT_FALSE(trie.empty());
}

TEST(Trie, FindForKeyPrefix) {
  Trie<int> trie;
  EXPECT_TRUE(trie.FindForKeyPrefix({}).empty());
  EXPECT_TRUE(trie.FindForKeyPrefix({1}).empty());
  EXPECT_TRUE(trie.FindForKeyPrefix({1, 2}).empty());
  EXPECT_TRUE(trie.FindForKeyPrefix({1, 2, 3}).empty());

  trie.Add({1, 2, 3}, 123);
  EXPECT_EQ((std::set<int>{123}), trie.FindForKeyPrefix({1}));

  trie.Add({1, 3}, 13);
  trie.Add({1, 2}, 12);
  EXPECT_EQ((std::set<int>{12, 13, 123}), trie.FindForKeyPrefix({1}));
  EXPECT_EQ((std::set<int>{13}), trie.FindForKeyPrefix({1, 3}));
  EXPECT_EQ((std::set<int>{12, 123}), trie.FindForKeyPrefix({1, 2}));
  EXPECT_EQ((std::set<int>{123}), trie.FindForKeyPrefix({1, 2, 3}));
  EXPECT_EQ(std::set<int>(), trie.FindForKeyPrefix({3}));
  EXPECT_EQ(std::set<int>(), trie.FindForKeyPrefix({9}));
}

}  // namespace dungeon
