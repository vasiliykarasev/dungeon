#include "data/interval_tree.h"

#include <algorithm>
#include <queue>

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace dungeon {

TEST(SimpleIntervalTreeNode, ThreeNodeCase) {
  // A tree consists of one interval, and then another interval is added.
  // After that, the tree consists of three nodes (one "virtual")
  std::shared_ptr<IntervalTreeNode> tree =
      IntervalTreeNode::Create(0, Interval(0.0, 1.0));
  EXPECT_TRUE(tree->IsLeaf());
  EXPECT_TRUE(tree->IsRoot());

  const std::shared_ptr<IntervalTreeNode> sibling =
      IntervalTreeNode::FindBestSibling(Interval(0.0, 0.5), tree);
  ASSERT_TRUE(!!sibling);

  IntervalTreeNode::Insert(1, Interval(0.0, 0.5), tree);

  auto root = IntervalTreeNode::Root(tree);
  EXPECT_EQ(root->interval, Interval(0.0, 1.0));
  ASSERT_TRUE(!!root->left_child);
  ASSERT_TRUE(!!root->right_child);
  EXPECT_EQ(root->left_child->interval, Interval(0.0, 1.0));
  EXPECT_EQ(root->right_child->interval, Interval(0.0, 0.5));
  ASSERT_TRUE(root->left_child->IsLeaf());
  ASSERT_EQ(0, *root->left_child->payload);
  ASSERT_TRUE(root->right_child->IsLeaf());
  ASSERT_EQ(1, *root->right_child->payload);
}

TEST(IntervalTreeNode, CreateTree) {
  std::vector<std::pair<int, Interval>> values{
      {0, {0.2, 0.4}}, {1, {0.4, 0.6}}, {2, {0.6, 0.8}}, {3, {0.8, 1.0}}};

  auto root = IntervalTreeNode::CreateTree(values);
  EXPECT_EQ(root->interval, Interval(0.2, 1.0));
  std::vector<const IntervalTreeNode*> leaves;
  std::queue<const IntervalTreeNode*> node_queue;
  node_queue.push(root.get());
  while (!node_queue.empty()) {
    const IntervalTreeNode* node = node_queue.front();
    node_queue.pop();
    if (node->IsLeaf()) {
      leaves.push_back(node);
    }
    if (node->left_child) {
      node_queue.push(node->left_child.get());
    }
    if (node->right_child) {
      node_queue.push(node->right_child.get());
    }
  }
  ASSERT_EQ(values.size(), leaves.size());
  // Order of the leaves is indeterminate, so sort by the leftmost point.
  std::sort(leaves.begin(), leaves.end(),
            [](const IntervalTreeNode* a, const IntervalTreeNode* b) {
              return a->interval.low <= b->interval.low;
            });
  for (size_t i = 0; i < values.size(); ++i) {
    ASSERT_EQ(values[i].second, leaves[i]->interval);
  }
}

}  // namespace dungeon
