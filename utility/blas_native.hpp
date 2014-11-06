#pragma once

#include <cmath>

namespace utility {

template <typename T> inline void cp(int N, const T *from, T *to) {
  for (int i = 0; i < N; ++i) {
    to[i] = from[i];
  }
}

template <typename T> inline void ax(int N, double a, const T *x, T *y) {
  for (int i = 0; i < N; ++i) {
    y[i] = a * x[i];
  }
}

template <typename T> inline void axpy(int N, double a, const T *x, T *y) {
  for (int i = 0; i < N; ++i) {
    y[i] += a * x[i];
  }
}

template <typename T, typename VAL = double>
inline VAL dot(int N, const T *x, const T *y) {
  VAL result = 0.0;
  for (int i = 0; i < N; ++i) {
    result += x[i] * y[i];
  }
  return result;
}

template <typename T, typename VAL = double>
inline VAL dot_even(int N, const T *x, const T *y) {
  VAL result = 0.0;
  for (int i = 0; i < N; i += 2) {
    result += x[i] * y[i];
  }
  return result;
}

template <typename T, typename VAL = double>
inline VAL dot_odd(int N, const T *x, const T *y) {
  VAL result = 0.0;
  for (int i = 1; i < N; i += 2) {
    result += x[i] * y[i];
  }
  return result;
}

template <typename T> inline void zero_clear(int N, T *x, T a = 0.0) {
  for (int i = 0; i < N; ++i) {
    x[i] = a;
  }
}

template <typename T> inline void scal(int N, double alpha, T *x) {
  for (int i = 0; i < N; ++i) {
    x[i] *= alpha;
  }
}

template <typename T> inline void mult(int N, const T *m, T *x) {
  for (int i = 0; i < N; ++i) {
    x[i] = x[i] * m[i];
  }
}

template <typename T, typename VAL = double>
inline VAL nrm2(const int N, const T *u) {
  return std::sqrt(dot(N, u, u));
}

template <typename T, typename VAL = double>
inline VAL nrm1(const int N, const T *u) {
  VAL result = 0.0;
  for (int i = 0; i < N; ++i) {
    result += std::abs(u[i]);
  }
  return result;
}

template <typename T> inline T nrminf(const int N, const T *u) {
  T max = u[0];
  for (int i = 1; i < N; ++i) {
    if (u[i] > max) {
      max = u[i];
    }
  }
  return max;
}

template <typename T, typename VAL = double>
inline VAL nrm22(const int N, const T *u) {
  return std::pow(nrm2(N, u), 2);
}

} // namespace utility
