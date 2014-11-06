#pragma once

#include <iterator>
#include <algorithm>
#include <cmath>

namespace utility {

template <typename Iterator>
using iterator_v = typename std::iterator_traits<Iterator>::value_type;
template <typename Iterator>
using iterator_d = typename std::iterator_traits<Iterator>::difference_type;

template <typename InputIterator>
inline void scal(InputIterator begin, InputIterator end,
                 iterator_v<InputIterator> alpha) {
  std::transform(begin, end, begin,
                 [=](iterator_v<InputIterator> x) { return alpha * x; });
}

template <class InputIterator>
inline void clear(InputIterator begin, InputIterator end,
                  iterator_v<InputIterator> zero = 0) {
  while (begin != end) {
    *begin++ = zero;
  }
}

/** square of L2-norm */
template <class InputIterator>
inline iterator_v<InputIterator> square_sum(InputIterator begin,
                                            InputIterator end) {
  typedef iterator_v<InputIterator> T;
  return std::accumulate(begin, end, T(0), [=](T x, T y) { return x + y * y; });
}

/** L2-norm */
template <class InputIterator>
inline double l2(InputIterator begin, InputIterator end) {
  return std::sqrt(square_sum(begin, end));
}

template <class InputIterator>
inline iterator_v<InputIterator> l1(InputIterator begin, InputIterator end) {
  typedef iterator_v<InputIterator> T;
  return std::accumulate(begin, end, T(0),
                         [=](T x, T y) { return x + std::abs(y); });
}

/** @f$ d(x,y) = \| x - y \|_{L^2} @f$ */
template <class InputIterator1, class InputIterator2>
inline double euclid_distance(InputIterator1 x_begin, InputIterator1 x_end,
                              InputIterator2 y_begin) {
  typedef iterator_v<InputIterator1> T1;
  typedef iterator_v<InputIterator2> T2;
  double sum = 0.0;
  while (x_begin != x_end) {
    T1 x = *x_begin++;
    T2 y = *y_begin++;
    sum += std::pow(x - y, 2);
  }
  return std::sqrt(sum);
}

/** @f$ \| x - y \| / (\| x \| + \| y \|) @f$
 *  if @f$ \| x \| == 0 @f$ or @f$ \| y \| == 0 @f$
 *  then returns @f$ \| x - y \| @f$
 */
template <class InputIterator1, class InputIterator2>
inline double relative_error(InputIterator1 x_begin, InputIterator1 x_end,
                             InputIterator2 y_begin) {
  typedef iterator_v<InputIterator1> T1;
  typedef iterator_v<InputIterator2> T2;
  double dif = 0.0;
  double nrm_x = 0.0;
  double nrm_y = 0.0;
  while (x_begin != x_end) {
    T1 x = *x_begin++;
    T2 y = *y_begin++;
    dif += std::pow(x - y, 2);
    nrm_x += std::pow(x, 2);
    nrm_y += std::pow(x, 2);
  }
  if (nrm_x > 0.0 && nrm_y > 0.0) {
    return std::sqrt(dif / (nrm_x + nrm_y));
  } else {
    return std::sqrt(dif);
  }
}

} // namespace utility
