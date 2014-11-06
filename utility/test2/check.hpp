#pragma once

#include "exception.hpp"

#include "../output.hpp"
#include "../blas.hpp"
#include "../log.hpp"
#include "../assert.hpp"

#include <cmath>
#include <fstream>
#include <vector>

namespace utility {
namespace test2 {

/** 真偽値が既に与えられている場合 */
inline void check(bool is /** テストすべき真偽値(Trueが正常) */,
                  const char *msg, const char *filename, int line,
                  const char *funcname) throw(Test2Error) {
  char id[1024];
  sprintf(id, "%s:%d:%s", filename, line, funcname);
  if (!is) {
    throw Test2Error(msg, id);
  } else {
    char message[1024];
    sprintf(message, "[TEST2] passed test %s", id);
    log(message, std::clog);
  }
}

/** 整数の一致をテストする */
inline void check_equal(int i0, int i1, const char *msg, const char *filename,
                        int line, const char *funcname) throw(Test2Error) {
  char id[1024];
  sprintf(id, "%s:%d:%s", filename, line, funcname);
  if (i0 != i1) {
    std::ofstream ofs(id);
    ofs << i0 << " " << i1 << std::endl;
    throw Test2Error(msg, id);
  } else {
    char message[1024];
    sprintf(message, "[TEST2] passed test %s", id);
    log(message, std::clog);
  }
}

/** 実数の一致をテストする */
inline void check_equal(double val, double ans, double threshold,
                        const char *msg, const char *filename, int line,
                        const char *funcname) throw(Test2RealCompareError) {
  UTILITY_ASSERT_CHECK(threshold > 0, "Threshold must be positive");

  char id[1024];
  sprintf(id, "%s:%d:%s", filename, line, funcname);

  if (!std::isfinite(val)) {
    throw Test2InfiniteError(msg, id);
  }

  double relative_error;
  if (std::abs(ans) <= threshold) {
    relative_error = std::abs(val);
  } else {
    relative_error = std::abs(val - ans) / ans;
  }

  if (relative_error > threshold) {
    std::ofstream ofs(id);
    ofs << std::scientific << std::setprecision(15);
    csv_wrap(ofs, " ") << val << ans << threshold;
    throw Test2RealCompareError(msg, id, threshold);
  } else {
    char message[1024];
    sprintf(message, "[TEST2] passed test %s", id);
    log(message, std::clog);
  }
}

/** 実数の配列が一致している事をテストする */
inline void check_equal(int N, const double *val, const double *ans,
                        double threshold, const char *msg, const char *filename,
                        int line, const char *funcname) {
  char id[1024];
  sprintf(id, "%s:%d:%s", filename, line, funcname);

  double res;
  for (int i = 0; i < N; ++i) {
    if (!std::isfinite(val[i])) {
      throw Test2InfiniteError(msg, id);
    }
  }
  if (ans == nullptr || nrm2(N, ans) == 0.0) {
    res = nrm2(N, val);
  } else {
    std::vector<double> tmp(N);
    cp(N, val, &tmp[0]);
    axpy(N, -1.0, ans, &tmp[0]);
    res = nrm2(N, &tmp[0]) / nrm2(N, ans);
  }

  if (res > N * threshold) {
    std::ofstream ofs(id);
    ofs << std::scientific << std::setprecision(15);
    log("Threshold", threshold, ofs);
    log("residual", res, ofs);
    if (ans == nullptr) {
      show(N, val, ofs);
    } else {
      show(N, val, ans, ofs);
    }
    throw Test2RealCompareError(msg, id, N * threshold);
  } else {
    char message[1024];
    sprintf(message, "[TEST2] passed test %s", id);
    log(message, std::clog);
  }
}
}
} // namespace utility::test2
