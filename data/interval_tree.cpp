#include "data/interval_tree.h"

#include <map>
#include <queue>

#include "glog/logging.h"

namespace dungeon {

std::unique_ptr<IntervalTreeNode> IntervalTreeNode::Create(
    int payload, const Interval& interval) {
  auto tree = std::make_unique<IntervalTreeNode>();
  tree->payload = std::make_unique<int>(payload);
  tree->interval = interval;
  return tree;
}

bool IntervalTreeNode::IsRoot() const { return !parent; }

bool IntervalTreeNode::IsLeaf() const { return !left_child && !right_child; }

std::shared_ptr<IntervalTreeNode> IntervalTreeNode::FindBestSibling(
    const Interval& interval, std::shared_ptr<IntervalTreeNode> root) {
  CHECK_NOTNULL(root);
  // This is a really stupid and silly algorithm: we basically just choose
  // the last child at a smallest level.
  // It's kind of an attempt to keep the tree balanced, but it's a bit
  // orthogonal to what we actually want to achieve.
  // Just meant as a stopgap.
  std::queue<std::pair<int, std::shared_ptr<IntervalTreeNode>>> leveled_nodes;
  std::map<int, std::shared_ptr<IntervalTreeNode>> leveled_leaves;
  leveled_nodes.push(std::make_pair(0, root));
  while (!leveled_nodes.empty()) {
    auto pr = leveled_nodes.front();
    leveled_nodes.pop();
    int level = pr.first;
    const std::shared_ptr<IntervalTreeNode> node = pr.second;
    if (node->IsLeaf()) {
      leveled_leaves[level] = node;
    } else {
      if (node->left_child) {
        leveled_nodes.push(std::make_pair(level + 1, node->left_child));
      }
      if (node->right_child) {
        leveled_nodes.push(std::make_pair(1, node->right_child));
      }
    }
  }
  //
  CHECK(!leveled_leaves.empty());
  return leveled_leaves.begin()->second;
}

void IntervalTreeNode::FixupIntervals(std::shared_ptr<IntervalTreeNode> node) {
  for (; static_cast<bool>(node); node = node->parent) {
    if (node->left_child && node->right_child) {
      node->interval = Interval::Union(node->left_child->interval,
                                       node->right_child->interval);
    } else if (node->left_child) {
      node->interval = node->left_child->interval;
    } else if (node->right_child) {
      node->interval = node->right_child->interval;
    }
  }
}

void IntervalTreeNode::Insert(int payload, const Interval& interval,
                              std::shared_ptr<IntervalTreeNode> root) {
  CHECK_NOTNULL(root);
  std::shared_ptr<IntervalTreeNode> sibling = FindBestSibling(interval, root);
  CHECK(sibling);
  std::shared_ptr<IntervalTreeNode> old_parent = std::move(sibling->parent);
  std::shared_ptr<IntervalTreeNode> new_parent =
      std::make_shared<IntervalTreeNode>();

  std::shared_ptr<IntervalTreeNode> new_leaf =
      IntervalTreeNode::Create(payload, interval);
  // Create a subtree consisting of the "sibling", new node, and "new parent".
  new_parent->left_child = sibling;
  new_parent->right_child = new_leaf;
  new_parent->left_child->parent = new_parent;
  new_parent->right_child->parent = new_parent;
  new_parent->interval = Interval::Union(sibling->interval, new_leaf->interval);

  // Fixup the old parent.
  if (old_parent) {
    new_parent->parent = old_parent;
    if (old_parent->left_child.get() == sibling.get()) {
      old_parent->left_child = new_parent;
    } else if (old_parent->right_child.get() == sibling.get()) {
      old_parent->right_child = new_parent;
    } else {
      LOG(FATAL) << "";
    }
  }
  // Now we need to walk from the node to the root, while fixing up all
  // intervals along the way.
  FixupIntervals(new_leaf);
}

std::shared_ptr<IntervalTreeNode> IntervalTreeNode::Root(
    std::shared_ptr<IntervalTreeNode> node) {
  CHECK_NOTNULL(node);
  while (node->parent) {
    node = node->parent;
  }
  return node;
}

std::shared_ptr<IntervalTreeNode> IntervalTreeNode::CreateTree(
    const std::vector<std::pair<int, Interval>>& values) {
  CHECK(!values.empty());
  std::shared_ptr<IntervalTreeNode> root = nullptr;
  for (const auto& [payload, interval] : values) {
    if (!root) {
      root = IntervalTreeNode::Create(payload, interval);
    } else {
      IntervalTreeNode::Insert(payload, interval, root);
      root = IntervalTreeNode::Root(root);
    }
  }
  return root;
}

}  // namespace dungeon
