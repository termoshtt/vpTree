#pragma once

#include <exception>
#include <string>
#include <cstdio>

namespace utility {
namespace test2 {

/*!
 * \class Test2Error exception.hpp "utility/test2/exception.hpp"
 * test2テストライブラリの基底例外
 */
struct Test2Error : public std::exception {
  char message[1024];
  Test2Error(const char *msg,
             const char *test_id /** identifier of test such */) {
    sprintf(message, "ERROR in %s %s", test_id, msg);
  }
  virtual const char *what() const throw() { return message; }
};

/*!
 * \class Test2RealCompareError exception.hpp "utility/test2/exception.hpp"
 * 実数値を比較した際に誤差が閾値以下の場合に出力されるエラー
 */
struct Test2RealCompareError : public Test2Error {
  const double threshold; ///< 誤差の閾値
  Test2RealCompareError(const char *msg, const char *test_id,
                        double threshold /** 誤差の閾値 */)
      : Test2Error(msg, test_id), threshold(threshold) {
    sprintf(message, "%s with threshold = %e", message, threshold);
  }
};

/*!
 * \class Test2InfiniteError exception.hpp "utility/test2/exception.hpp"
 * 数値が発散している場合に投げられるエラー
 */
struct Test2InfiniteError : public Test2Error{
  Test2InfiniteError(const char* msg, const char *test_id)
    :Test2Error(msg, test_id) {};
};

static const double strict_threshold =
    1.0e-14; ///< 実数が完全に一致したと見做す閾値
}
} // namespace utility::test2
