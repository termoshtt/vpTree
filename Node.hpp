#pragma once

#include "utility/assert.hpp"

#include <vector>
#include <memory>
#include <algorithm>

namespace utility {
namespace tree {
namespace vp {

template <typename Index> class Node;
template <typename Index> class LeafNode;

template <typename Index> using pNode = std::unique_ptr<Node<Index> >;
template <typename Index> using pLeafNode = std::unique_ptr<LeafNode<Index> >;

/*!
 * \class Node Node.hpp "utility/tree/vp/Node.hpp"
 * non-leaf node
 *
 * - 一つのPointだけ持つ
 * - 子は全部non-leafかleafのどちらか(混合はゆるさない)
 */
template <typename Index> class Node {
  typedef Node<Index> THIS;

public:
  const unsigned short m;
  Node<Index> *parent; // weak reference
  Index vp;
  std::vector<float> mu;
  std::vector<pNode<Index> > children;
  std::vector<pLeafNode<Index> > leaves;

  Node(unsigned short m /** branching factor */, Index vp_idx)
      : m(m), parent(nullptr), vp(vp_idx) {}

  /** このノード以下が完全に埋まっているかどうか */
  bool is_full() const {
    if (!children.empty()) {
      for (auto &n : children) {
        if (!n->is_full())
          return false;
      }
      return true;
    } else {
      for (auto &l : leaves) {
        if (!l->is_full())
          return false;
      }
      return true;
    }
  }

  /** @f$ \mu_{i-1} < x < \mu_i @f$を満すiを返す
   *
   * Tested
   * ------
   *  test/Node.cpp:test_get_covering_node()
   *    - @f$ \mu = {0.2, 0.3} @f$として以下を確認
   *      - @f$ x=0.0 @f$ -> 0
   *      - @f$ x=0.25 @f$ -> 1
   *      - @f$ x=0.3.5 @f$ -> 2
   */
  unsigned short get_covering_node(float x) const {
    if (mu.empty()) {
      return 0;
    }
    unsigned short i = 0;
    for (; i < mu.size(); ++i) {
      if (x < mu[i]) {
        break;
      }
    }
    return i;
  }

  /** 新しいノードを追加する */
  Node<Index> *new_Node(unsigned short m, Index idx) {
    UTILITY_ASSERT_CHECK(children.size() < m, "branching factor");
    UTILITY_ASSERT_CHECK(leaves.size() == 0,
                         "Node<Index> should has children only or leaves only");
    auto *node = new Node<Index>(m, idx);
    node->parent = this;
    children.emplace_back(node);
    return node;
  }

  /** 新しい葉ノードを追加する */
  LeafNode<Index> *new_LeafNode(unsigned short l) {
    UTILITY_ASSERT_CHECK(leaves.size() < m, "branching factor");
    UTILITY_ASSERT_CHECK(children.size() == 0,
                         "Node<Index> should has children only or leaves only");
    auto *leaf = new LeafNode<Index>(l, *this);
    leaves.emplace_back(leaf);
    return leaf;
  }
};

/*!
 * \class LeafNode Node.hpp "utility/tree/vp/Node.hpp"
 * leaf node
 */
template <typename Index> class LeafNode {
public:
  const unsigned short l;
  Node<Index> &parent;
  std::vector<Index> points;

  LeafNode(unsigned short l /** max capacity of leaf node */,
           Node<Index> &parent)
      : l(l), parent(parent) {}

  void add(Index idx) {
    UTILITY_ASSERT_CHECK(points.size() < l, "size of leaf");
    points.push_back(idx);
  }
  bool is_full() const { return points.size() >= l; }
};

} // namespace vp
} // namespace tree
} // namespace utility
