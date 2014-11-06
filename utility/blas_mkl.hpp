#ifndef BLAS_MKL_20120601_INC
#define BLAS_MKL_20120601_INC

#include "mkl.h" /* use Intel Math Kernel Library */

namespace utility {

inline void cp(int N, const double *from, double *to) {
  cblas_dcopy(N, from, 1, to, 1);
}
inline void axpy(int N, double a, const double *x, double *y) {
  cblas_daxpy(N, a, x, 1, y, 1);
}
inline double dot(int N, const double *x, const double *y) {
  return cblas_ddot(N, x, 1, y, 1);
}
inline void scal(int N, double alpha, double *x) {
  cblas_dscal(N, alpha, x, 1);
}
inline void mult(int N, const double *m, double *x) {
  // TODO BLAS level 2が使えそう。
  for (int i = 0; i < N; ++i) {
    x[i] = x[i] * m[i];
  }
}
inline double nrm2(const int N, const double *u) {
  return cblas_dnrm2(N, u, 1);
}
inline double nrm1(const int N, const double *u) {
  return cblas_dasum(N, u, 1);
}
inline double nrminf(const int N, const double *u) {
  // TODO これもmklに置き換えられる
  double max = u[0];
  for (int i = 1; i < N; ++i) {
    if (u[i] > max) {
      max = u[i];
    }
  }
  return max;
}
inline double nrm22(const int N, const double *u) {
  return cblas_ddot(N, u, 1, u, 1);
}

} // namespace utility

#endif /* ----- #ifndef BLAS_MKL_20120601_INC  ----- */
