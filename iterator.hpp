#pragma once

#include "utility/assert.hpp"

#include <vector>
#include <memory>
#include <algorithm>

namespace vpTree {

/*!
 * @class TreeIterator iterator.hpp "vpTree/iterator.hpp"
 * @brief vp-treeを巡回するイテレータ
 *
 * 状態としてありえるのは
 *  - Nodeを指している
 *  - LeafNodeを指している
 *    - now番目のIndexを指している
 */
template <typename Index> class TreeIterator {
  typedef TreeIterator<Index> THIS;

  Node<Index> *const root;
  Node<Index> *n;
  LeafNode<Index> *l;
  unsigned short now;

public:
  typedef Index value_type;

  /** for end() */
  TreeIterator() : root(nullptr), n(nullptr), l(nullptr), now(0) {}
  /** for begin() */
  TreeIterator(Node<Index> *root /** このノード以下を巡回する */)
      : root(root), n(root), l(nullptr), now(0) {}

  value_type operator*() const {
    UTILITY_ASSERT_CHECK(n || l, "Range overrun");
    if (n)
      return n->vp;
    return l->points[now];
  }

  /** 現在指しているポインタがNodeであれば真 */
  bool is_Node() const {
    if (n)
      return true;
    return false;
  }

  /** 現在指しているポインタがLeafNodeであれば真 */
  bool is_LeafNode() const {
    if (l)
      return true;
    return false;
  }

  /** 現在指しているNodeのポインタを取得する。
   *
   * - is_Node() がtrueの時だけ有効
   *  ポインタが有効かどうかは使用者の責任
   */
  Node<Index> *get_Node_ptr() const { return n; }
  /** 現在指しているLeafNodeのポインタを取得する。
   *
   * - is_LeafNode() がtrueの時だけ有効
   *  ポインタが有効かどうかは使用者の責任
   */
  LeafNode<Index> *get_LeafNode_ptr() const { return l; }

  /** rootが一致していなくてもいい */
  bool operator==(THIS &rhs) const {
    return ((n == rhs.n) && (l == rhs.l) && (now == rhs.now));
  }

  bool operator!=(THIS &rhs) const { return !this->operator==(rhs); }

  THIS operator++(int) {
    THIS tmp = *this;
    ++*this;
    return tmp;
  }

  THIS &operator++() {
    if ((!n) && (!l))
      return *this;
    /* if Node */
    if (n) {
      // node指してる時は下にしか降りない
      if (n->children.empty()) {
        l = n->leaves[0].get();
        n = nullptr;
        now = 0;
      } else {
        n = n->children[0].get();
      }
      return *this;
    }
    /* if Leaf */
    if (++now < l->points.size())
      return *this; // 今のleafの次
    Node<Index> *p = &l->parent;
    for (unsigned short m = 0; m < p->m; ++m) {
      if (p->leaves[m].get() == l && m < p->m - 1) {
        l = p->leaves[m + 1].get();
        now = 0;
        return *this; // 次のleaf
      }
    }
    Node<Index> *pp = p->parent;
    while (true) {
      if (!pp || p == root) {
        l = nullptr;
        now = 0;
        return *this; // 最後のリーフ (iteration end)
      }
      for (unsigned short m = 0; m < pp->m; ++m) {
        if (pp->children[m].get() == p && m < pp->m - 1) {
          n = pp->children[m + 1].get();
          l = nullptr;
          now = 0;
          return *this; // 次のnode
        }
      }
      p = pp;
      pp = pp->parent;
    }
  }
};

template <typename Index> TreeIterator<Index> begin(Node<Index> *n) {
  return TreeIterator<Index>(n);
}

template <typename Index> TreeIterator<Index> end(Node<Index> *) {
  return TreeIterator<Index>();
}

} // namespace vpTree
