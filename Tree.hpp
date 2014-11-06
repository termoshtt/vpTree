#pragma once

#include "Node.hpp"
#include "Omega.hpp"
#include "insert.hpp"
#include "construct.hpp"
#include "iterator.hpp"
#include "distance.hpp"

#include <tuple>

namespace utility {
namespace tree {
namespace vp {

/*!
 * @class Tree Tree.hpp "utility/tree/vp/Tree.hpp"
 * @brief vantage-point tree
 *
 * このライブラリが提供している機能を使いやすいようにラップしたクラス。
 * テストが行いやすいように各機能は()独立して実装してあるが、
 * 実用上はこのクラスだけを用いれば十分である。
 *
 * Feature
 * -------
 *  - 要素数が少数の場合は木を構成せずに線形に探索する。
 *
 */
template <class Point, typename Index> class Tree {
  const unsigned short M, L;
  const unsigned long max_linear;
  const unsigned int n_trial, m_random;

  pNode<Index> root;
  Omega<Point, Index> omega;
  std::vector<Index> indices;

public:
  Tree(unsigned short M /** branching factor */,
       unsigned short L /** number of point in a leaf node */,
       unsigned long max_linear /** 線形に探索する最大数 */,
       unsigned int n_trial /** select_vpにおける試行回数 */,
       unsigned int m_random /** select_vpにおける比較にしようする個数 */)
      : M(M), L(L), max_linear(max_linear), n_trial(n_trial),
        m_random(m_random), indices(0) {
    UTILITY_ASSERT_CHECK(max_linear >= M,
                         "There must be M indices to create a tree");
    indices.reserve(max_linear);
  }

  /** 点が保持されているか */
  bool empty() const { return indices.empty() && !root; }

  /** 点xの最近接要素の取得を試みる。
   *
   * @f$ \tau @f$ が小さすぎると失敗する。
   * See also vp::get_nearest
   */
  Nearest<Index> get_nearest(const Point &x, float tau) const {
    UTILITY_ASSERT_CHECK(!empty(), "There is no point.");
    if (!root)
      return vp::choose_nearest(omega, indices, x);
    return vp::get_nearest(omega, root.get(), x, tau);
  }

  /** 要素を挿入する */
  Index insert(const Point &x) {
    Index idx = omega.insert(x);
    if (root) {
      vp::insert(omega, root, idx, M, L, n_trial, m_random);
    } else {
      indices.push_back(idx);
      if (indices.size() > max_linear) {
        root.reset(construct(omega, indices, M, L, n_trial, m_random));
        indices.clear();
      }
    }
    return idx;
  }

  /** 対応するデータ点を取得 */
  const Point &get(Index idx) const { return omega[idx]; }

  /** Omega を取得する */
  const Omega<Point, Index> &get_Omega() const { return omega; }

  /** 木を構成しているかどうか */
  bool tree_constructed() const { return bool(root); }

  Index size() const {
    if (tree_constructed())
      return omega.size();
    return indices.size();
  }

  /* iterators */
  typedef TreeIterator<Index> iterator;
  /** tree_constructed() が真の時だけ有効 */
  iterator begin() const { return iterator(root.get()); }
  /** tree_constructed() が真の時だけ有効 */
  iterator end() const { return iterator(); }

  /** DOT言語で木を出力する */
  void output_dot(std::ostream &ost) const { show_dot(root.get(), ost); }
  /** DOT言語で木を出力する */
  void output_dot(const std::string &filename) const {
    show_dot(root.get(), filename);
  }
};

} // namespace vp
} // namespace tree
} // namespace utility
