#pragma once

#include "exception.hpp"
#include <cmath>

namespace utility {
namespace assert {

#define UTILITY_ASSERT_CHECK(expr, msg)                                        \
  utility::assert::check(expr, msg, __FILE__, __func__, __LINE__)
inline void check(bool expr, const char *msg, const char *filename,
                  const char *funcname, int line) {
  if (!expr) {
    throw AssertionFailed(msg, filename, funcname, line);
  }
}

#define UTILITY_ASSERT_EQUAL_ARRAYS(N, arr1, arr2, TH)                         \
  utility::assert::equal_arrays(N, arr1, arr2, TH, __FILE__, __func__, __LINE__)
inline void equal_arrays(int N, const double *arr1, const double *arr2,
                         double threshold, const char *filename,
                         const char *funcname, int line) {
  double dif = 0.0;
  for (int i = 0; i < N; ++i) {
    dif += std::pow(arr1[i] - arr2[i], 2);
  }
  if (dif / N > threshold) {
    throw AssertionFailed("equal_arrays", filename, funcname, line);
  }
}

#define UTILITY_ASSERT_SMALL_ENOUGH(err, TH)                                   \
  utility::assert::small_enough(err, TH, __FILE__, __func__, __LINE__)
inline void small_enough(double err, double threshold, const char *filename,
                         const char *funcname, int line) {
  if (err > threshold) {
    throw AssertionFailed("small_enough", filename, funcname, line);
  }
}
}
} // namespace utility::assert
