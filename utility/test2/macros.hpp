#pragma once

/**
 * \defgroup TEST2_MACROS
 * @{
 */
#define TEST2_CHECK(is)                                                        \
  utility::test2::check(is, "", __FILE__, __LINE__, __func__)
#define TEST2_CHECK_MSG(is, msg)                                               \
  utility::test2::check(is, msg, __FILE__, __LINE__, __func__)
#define TEST2_CHECK_EQUAL_INT(val, ans)                                        \
  utility::test2::check_equal(val, ans, "", __FILE__, __LINE__, __func__)
#define TEST2_CHECK_EQUAL_INT_MSG(val, ans, msg)                               \
  utility::test2::check_equal(val, ans, msg, __FILE__, __LINE__, __func__)
#define TEST2_CHECK_EQUAL_REAL(val, ans)                                       \
  utility::test2::check_equal(val, ans, utility::test2::strict_threshold, "",  \
                              __FILE__, __LINE__, __func__)
#define TEST2_CHECK_EQUAL_REAL_MSG(val, ans, msg)                              \
  utility::test2::check_equal(val, ans, utility::test2::strict_threshold, msg, \
                              __FILE__, __LINE__, __func__)
#define TEST2_CHECK_EQUAL_REAL_TH(val, ans, threshold)                         \
  utility::test2::check_equal(val, ans, threshold, "", __FILE__, __LINE__,     \
                              __func__)
#define TEST2_CHECK_EQUAL_REAL_TH_MSG(val, ans, threshold, msg)                \
  utility::test2::check_equal(val, ans, threshold, msg, __FILE__, __LINE__,    \
                              __func__)
#define TEST2_CHECK_EQUAL_ARRAY(N, val, ans)                                   \
  utility::test2::check_equal(N, val, ans, utility::test2::strict_threshold,   \
                              "", __FILE__, __LINE__, __func__)
#define TEST2_CHECK_EQUAL_ARRAY_MSG(N, val, ans, msg)                          \
  utility::test2::check_equal(N, val, ans, utility::test2::strict_threshold,   \
                              msg, __FILE__, __LINE__, __func__)
#define TEST2_CHECK_EQUAL_ARRAY_TH(N, val, ans, threshold)                     \
  utility::test2::check_equal(N, val, ans, threshold, "", __FILE__, __LINE__,  \
                              __func__)
#define TEST2_CHECK_EQUAL_ARRAY_TH_MSG(N, val, ans, threshold, msg)            \
  utility::test2::check_equal(N, val, ans, threshold, msg, __FILE__, __LINE__, \
                              __func__)
/** @} */
