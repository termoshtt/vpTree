#pragma once

#include "utility/algorithm.hpp"

namespace vpTree {

/** 距離関数([0:1]に規格化済み)
 *
 * 自作クラスをPointに使用する場合はこの距離関数を特殊化する。
 */
template <class Point> float d(const Point &x, const Point &y) {
  float ed = utility::euclid_distance(x.begin(), x.end(), y.begin());
  return ed / (1.0 + ed);
}

} // namespace vpTree
