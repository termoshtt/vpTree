#ifndef BLAS_20120601_INC
#define BLAS_20120601_INC

#ifdef USE_INTEL_MKL
#include "blas_mkl.hpp"
#else
#include "blas_native.hpp"
#endif

#endif /* ----- #ifndef BLAS_20120601_INC  ----- */
