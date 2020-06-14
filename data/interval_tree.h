#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "data/interval.h"

namespace dungeon {

// A data structure for queries like "given a collection of intervals
// (representing start/end points of some events), find all intervals that
// overlap with a point."
// Note: this implementation is based on a AABB tree (an interval on the real
// line is an axis-aligned bounding box in 2D).
//
// NOTE: THIS IMPLEMENTATION IS NOT FINISHED.

// A node in the IntervalTree.
struct IntervalTreeNode {
  std::shared_ptr<IntervalTreeNode> left_child;
  std::shared_ptr<IntervalTreeNode> right_child;
  std::shared_ptr<IntervalTreeNode> parent;

  std::unique_ptr<int> payload;
  Interval interval;

  // Creates a tree from a collection of values.
  static std::shared_ptr<IntervalTreeNode> CreateTree(
      const std::vector<std::pair<int, Interval>>& values);

  // Creates a single node.
  static std::unique_ptr<IntervalTreeNode> Create(int payload,
                                                  const Interval& interval);

  // Inserts a node into the tree.
  static void Insert(int payload, const Interval& interval,
                     std::shared_ptr<IntervalTreeNode> root);

  // Finds the best sibling for a given interval. The second argument is the
  // root of the tree.
  static std::shared_ptr<IntervalTreeNode> FindBestSibling(
      const Interval& interval, std::shared_ptr<IntervalTreeNode> tree);

  // Traverses the tree "up" from the given node, and fixes up intervals.
  static void FixupIntervals(std::shared_ptr<IntervalTreeNode> node);

  // Given a node in the tree, returns the root. If the note has no parents,
  // then it must be the rort, and is returned.
  static std::shared_ptr<IntervalTreeNode> Root(
      std::shared_ptr<IntervalTreeNode> node);

  // Returns true if the node is a leaf.
  bool IsLeaf() const;

  // Returns true if the node is the root of the tree.
  bool IsRoot() const;
};

}  // namespace dungeon
