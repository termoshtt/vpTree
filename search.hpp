#pragma once

#include "construct.hpp"

namespace vpTree {

/** 任意の点の最近接の LeafNode を求める */
template <typename Point, typename Index>
LeafNode<Index> *
get_nearest_leaf(const Omega<Point, Index> &omega,
                 const Node<Index> *n /** 検索を開始するノード */,
                 const Point &p /** 検索する点 */) {
  while (true) {
    float x = d(omega[n->vp], p);
    unsigned short i = n->get_covering_node(x);
    if (!n->children.empty()) {
      n = n->children.at(i).get();
      continue;
    }
    return n->leaves.at(i).get();
  }
}

/** Omega に登録した点の最近接の LeafNode を求める */
template <typename Point, typename Index>
inline LeafNode<Index> *
get_nearest_leaf(const Omega<Point, Index> &omega,
                 const Node<Index> *n /** 検索を開始するノード */,
                 Index idx /** 検索する点のインデックス */) {
  return get_nearest_leaf(omega, n, omega[idx]);
}

template <typename Index> using Nearest = std::tuple<Index, float>;

template <typename Index>
inline Nearest<Index> min(Nearest<Index> a, Nearest<Index> b) {
  if (std::get<1>(a) > std::get<1>(b))
    return b;
  return a;
}

template <typename Index>
inline Nearest<Index> min(Nearest<Index> a, Index idx, float d) {
  return min(a, Nearest<Index>(idx, d));
}

template <typename Point, typename Index>
inline Nearest<Index> choose_nearest(const Omega<Point, Index> &omega,
                                     const std::vector<Index> &list,
                                     const Point &x) {
  Nearest<Index> n(0, 1.0);
  for (Index idx : list) {
    float dis = d(x, omega[idx]);
    n = min(n, idx, dis);
  }
  return n;
}

/** ある点pの最近接要素を取得する
 *
 * - @f$ d(p, q) < \tau @f$以内にある点qを取得する事を試みる。
 * - ただし上述した点が無い場合には検索した中での
 *   最近接点@f$ q^\prime @f$ を返す。
 *   この点は真の最近接であるとは限らない。
 * - @f$ \tau = @f$ d(p, q^\prime) @f$は確実に見つかる。
 */
template <typename Point, typename Index>
inline Nearest<Index>
get_nearest(const Omega<Point, Index> &omega,
            const Node<Index> *n /** 検索を開始するノード */,
            const Point &p /** 検索する点 */, float tau) {
  Nearest<Index> near(0, 1.0);
  float x = d(omega[n->vp], p);
  near = min(near, n->vp, x);
  for (unsigned short m = 0; m < n->m; ++m) {
    float u_band = (m < n->mu.size() ? n->mu[m] : 1.0);
    float l_band = (m > 0 ? n->mu[m - 1] : 0.0);
    if (x - tau > u_band || x + tau < l_band)
      continue;
    if (!n->children.empty()) {
      auto *cn = n->children.at(m).get();
      auto n_near = get_nearest(omega, cn, p, tau);
      near = min(near, n_near);
    } else {
      auto *l = n->leaves.at(m).get();
      auto l_near = choose_nearest(omega, l->points, p);
      near = min(near, l_near);
    }
  }
  return near;
}

} // namespace vpTree
