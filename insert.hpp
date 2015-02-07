#pragma once

#include "construct.hpp"
#include "iterator.hpp"
#include "search.hpp"

namespace vpTree {

/**
 * あるノード以下にあるインデックスを全て集める
 */
template <typename Index> inline std::vector<Index> gather(Node<Index> *n) {
  auto b = begin(n);
  auto e = end(n);
  std::vector<Index> indices;
  while (b != e) {
    indices.push_back(*b++);
  }
  return std::move(indices);
}

/** 点を木に挿入する
 *
 * Tested
 * ------
 *  test/insert.cpp
 */
template <typename Point, typename Index>
inline void insert(Omega<Point, Index> &omega, pNode<Index> &root, Index idx,
                   unsigned short M /** branching factor */,
                   unsigned short L /** size of leaf node */,
                   unsigned int n_trial /** 試行回数 */,
                   unsigned int m_random /** 比較する個数 */) {
  LeafNode<Index> *l = get_nearest_leaf(omega, root.get(), idx);
  if (!l->is_full()) {
    l->add(idx);
    return;
  }
  Node<Index> *n = &(l->parent);
  while (n->is_full() && n->parent) {
    n = n->parent; // 空があるか、rootに到達するまで遡る
  }
  std::vector<Index> indices = gather(n);
  indices.push_back(idx);
  Node<Index> *nn = construct(omega, indices, M, L, n_trial, m_random);
  if (!n->parent) { // 既にルートに逹っしている場合
    root.reset(nn);
    return;
  }
  for (auto &pn : n->parent->children) {
    if (pn.get() == n) {
      nn->parent = n->parent;
      pn.reset(nn);
      return;
    }
  }
  check(0, "Unknown error");
}

/** 点を木に挿入する */
template <typename Point, typename Index>
inline void insert(Omega<Point, Index> &omega, pNode<Index> &root,
                   const Point &p, unsigned short M /** branching factor */,
                   unsigned short L /** size of leaf node */,
                   unsigned int n_trial /** 試行回数 */,
                   unsigned int m_random /** 比較する個数 */) {
  Index idx = omega.insert(p);
  insert(omega, root, idx, M, L, n_trial, m_random);
}

} // namespace vpTree
