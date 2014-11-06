#pragma once

#include "Node.hpp"
#include "Omega.hpp"
#include "utility/random.hpp"

#include <algorithm>

namespace vpTree {

/** vpを選ぶ
 *
 * Procedure
 * ---------
 *  - n個の候補をランダムに選ぶ
 *  - m個のサンプルをランダムに選ぶ
 *  - 分散が大きいものを選ぶ
 *
 * Tested
 * ------
 *  test/construct.cpp
 *   - `{0, -1, 1, 4, 2}`で4が選択される事を確認
 */
template <typename Point, typename Index>
inline Index select_vp(Omega<Point, Index> &omega,
                       const std::vector<Index> &indices,
                       unsigned int n_trial /** 試行回数 */,
                       unsigned int m_random /** 比較する個数 */) {
  using utility::random::select_n;
  unsigned int N = indices.size();
  n_trial = std::min(N, n_trial);
  m_random = std::min(N, m_random);
  auto trials = select_n(indices.begin(), indices.end(), n_trial);
  auto samples = select_n(indices.begin(), indices.end(), m_random);
  double max_var = 0.0;
  Index max_idx = 0;
  for (Index t : trials) {
    double mean = 0.0;
    double moment = 0.0;
    unsigned int skip = 0;
    for (Index s : samples) {
      if (s == t) {
        skip++;
        continue;
      }
      double d = omega.d(t, s);
      mean += d;
      moment += d * d;
    }
    mean /= samples.size() - skip;
    moment /= samples.size() - skip;
    double var = moment - mean * mean;
    if (var > max_var) {
      max_var = var;
      max_idx = t;
    }
  }
  return max_idx;
}

/** 点列を登録する */
template <typename Point, typename Index>
std::vector<Index> register_points(Omega<Point, Index> &omega,
                                   const std::vector<Point> &points) {
  std::vector<Index> indices;
  indices.reserve(points.size());
  for (const Point &p : points) {
    indices.push_back(omega.insert(p));
  }
  return std::move(indices);
}

/*!
 * @struct PreNode construct.hpp "vpTree/construct.hpp"
 * @brief 距離でインデックスをソートするための構造体
 */
template <typename Index> struct PreNode {
  Index idx;
  float d;
  bool operator<(const PreNode &n) const { return (d < n.d); }
};

/** 木を構成する
 *
 * - 子は Node か LeafNode のいずれかのみ。混合はゆるさない。
 * - 子は M 個(branching factor)分一気に作る
 * - 一番上は Node なので最低M+1個のインデックスが必要
 * - M分割したインデックスが LeafNode に入らず Nodeを構成するには少ない、
 *   とならない様にするためには L>M が必要
 *
 * Tested
 * ------
 *  test/construct.cpp
 *   - 一次元の場合にうまく構成できている事を目視で確認
 */
template <typename Point, typename Index>
Node<Index> *construct(Omega<Point, Index> &omega,
                       const std::vector<Index> &indices,
                       unsigned short M /** branching factor */,
                       unsigned short L /** size of leaf node */,
                       unsigned int n_trial /** 試行回数 */,
                       unsigned int m_random /** 比較する個数 */) {
  UTILITY_ASSERT_CHECK(M >= 2, "branching factor must be larger than 2");
  UTILITY_ASSERT_CHECK(indices.size() >= (size_t)(M + 1),
                       "Too small indices to create node");
  UTILITY_ASSERT_CHECK(L > M, "L must be exactly larger than M");
  const size_t count = indices.size() - 1;

  /* vpを決めてソートしておく */
  std::vector<PreNode<Index> > work;
  work.reserve(count);
  Index vp = select_vp(omega, indices, n_trial, m_random);
  for (Index i : indices) {
    if (i == vp)
      continue;
    work.push_back({ i, omega.d(i, vp) });
  }
  std::sort(work.begin(), work.end());

  /* 木を構成する */
  Node<Index> *root = new Node<Index>(M, vp);
  /* NをM分割するにはN=pM+rとした時,
   * p+1をr個、pをM-r個用意すればいい */
  size_t p = count / M;
  unsigned short r = count % M;
  auto itr = work.begin();
  size_t max_each = (r == 0 ? p : p + 1); // 各ノードに入れる予定の最大数
  for (unsigned int m = 0; m < M; ++m) {
    size_t size = (m < r ? p + 1 : p);
    std::vector<Index> sub_indices(size);
    for (unsigned int s = 0; s < size; ++s) {
      sub_indices[s] = itr[s].idx;
    }
    itr += size;
    if (itr != work.end()) {
      root->mu.push_back(itr->d);
    }

    /* 子ノードの生成 */
    if (max_each > L) {
      Node<Index> *n = construct(omega, sub_indices, M, L, n_trial, m_random);
      n->parent = root;
      root->children.emplace_back(n);
    } else {
      LeafNode<Index> *l = new LeafNode<Index>(L, *root);
      l->points.resize(sub_indices.size());
      std::copy(sub_indices.begin(), sub_indices.end(), l->points.begin());
      root->leaves.emplace_back(l);
    }
  }
  return root;
}

} // namespace vpTree
