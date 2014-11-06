#pragma once

#include "distance.hpp"
#include "utility/assert.hpp"

#include <vector>
#include <deque>
#include <tuple>
#include <boost/range/algorithm/find.hpp>

namespace utility {
namespace tree {
namespace vp {

template <class Point, typename Index> class OmegaIterator;

/**
 * @class Omega Omega.hpp "utility/tree/vp/Omega.hpp"
 * 考えている点の集合 @f$ \Omega = \{ x_i \} @f$
 *
 * - 削除後にもインデックス @f$ i @f$ を無効化しない
 * - 削除されたインデックスは後の挿入で再利用される
 *
 * Tested
 * ------
 *  test/Omega.cpp
 *    - 削除後の挿入で正しく使用されていないインデックスを使用している事を確認
 *    - 管理外のインデックスに対して例外を投げる事を確認
 */
template <class Point, typename Index> class Omega {
  std::vector<Point> points;
  std::deque<Index> deleted;

public:
  /** 新に点を追加する */
  Index insert(const Point &x) {
    if (deleted.empty()) {
      points.push_back(x);
      return points.size() - 1;
    }
    Index idx = deleted.at(0);
    deleted.pop_front();
    points[idx] = x;
    return idx;
  }

  /** 既存の点を廃棄する。
   *  実際に削除されるのは次の挿入で上書きされる時。 */
  void erase(Index idx) {
    check_deleted(idx);
    deleted.push_back(idx);
  }

  /** 管理している点同士の距離を計算する */
  float d(Index i, Index j) const {
    check_deleted(i);
    check_deleted(j);
    return vp::d(points.at(i), points.at(j));
  }

  /** 点を取得する */
  const Point &operator[](Index idx) const {
    check_deleted(idx);
    return points.at(idx);
  }

  /** 登録されている有効な点の数 */
  Index size() const { return points.size() - deleted.size(); }

  typedef OmegaIterator<Point, Index> iterator;
  friend iterator;
  iterator begin() { return iterator(*this); }
  iterator end() { return iterator(*this, points.size()); }

private:
  void check_deleted(Index idx) const {
    using boost::range::find;
    if (!deleted.empty()) {
      auto itr = find(deleted, idx);
      UTILITY_ASSERT_CHECK(itr != deleted.end(), "deleted index");
    }
  }
};

/*!
 * @class OmegaIterator Omega.hpp "utility/tree/vp/Omega.hpp"
 * @brief Omega で管理されている要素を巡回する
 */
template <class Point, typename Index> class OmegaIterator {
  typedef OmegaIterator THIS;
  const std::vector<Point> &p;
  const std::deque<Index> &d;
  Index now;

public:
  typedef std::tuple<Index, Point> value_type;

  value_type operator*() const { return std::make_tuple(now, p.at(now)); }

  OmegaIterator(const Omega<Point, Index> &o, Index start = 0)
      : p(o.points), d(o.deleted), now(start) {}

  THIS operator++(int) {
    THIS tmp = *this;
    ++*this;
    return tmp;
  }
  THIS &operator++() {
    using boost::range::find;
    if (now >= p.size())
      return *this;
    now++;
    auto itr = find(d, now);
    if (itr != d.end())
      return this->operator++();
    return *this;
  }
  bool operator==(THIS &rhs) const { return now == rhs.now; }
  bool operator!=(THIS &rhs) const { return !this->operator==(rhs); }
};

} // namespace vp
} // namespace tree
} // namespace utility
