#pragma once

#include "common.hpp"
#include "assert.hpp"

#include <random>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>

namespace utility {
namespace random {

/** get the value read from /dev/urandom (unlocked, relatively fast) */
template <typename T> T get_dev_urandom() {
  T val;
  std::ifstream dev_rand("/dev/urandom", std::ios::binary);
  if (dev_rand.is_open()) {
    dev_rand.read(reinterpret_cast<char *>(&val), sizeof(T));
    dev_rand.close();
  }
  return val;
}

/** get the value read from /dev/random (too slow but highly random) */
template <typename T> T get_dev_random() {
  T val;
  std::ifstream dev_rand("/dev/random", std::ios::binary);
  if (dev_rand.is_open()) {
    dev_rand.read(reinterpret_cast<char *>(&val), sizeof(T));
    dev_rand.close();
  }
  return val;
}

inline std::seed_seq seed(unsigned int len = 10) {
  std::random_device rnd;
  std::vector<std::uint_least32_t> v(len);
  std::generate(v.begin(), v.end(), std::ref(rnd));
  std::seed_seq s(v.begin(), v.end());
  return s;
}

template <class InputIterator, class Distribution, class Generator>
inline void fill(InputIterator begin, InputIterator end, Distribution &dist,
                 Generator &gen) {
  while (begin != end) {
    *begin++ = dist(gen);
  }
}

template <class InputIterator, class Distribution>
inline void fill(InputIterator begin, InputIterator end, Distribution &dist) {
  auto s = seed();
  std::mt19937 gen(s);
  while (begin != end) {
    *begin++ = dist(gen);
  }
}

template <typename T, class Distribution, class Generator>
inline void fill(int N, T *arr, Distribution &dist, Generator &gen) {
  fill(arr, arr + N, dist, gen);
}

template <typename T, class Distribution>
inline void fill(int N, T *arr, Distribution &dist) {
  fill(arr, arr + N, dist);
}

namespace traits {
template <typename T> struct uniform_distribution;

template <> struct uniform_distribution<double> {
  typedef std::uniform_real_distribution<double> type;
};
template <> struct uniform_distribution<float> {
  typedef std::uniform_real_distribution<float> type;
};
template <> struct uniform_distribution<short> {
  typedef std::uniform_int_distribution<short> type;
};
template <> struct uniform_distribution<int> {
  typedef std::uniform_int_distribution<int> type;
};
template <> struct uniform_distribution<long> {
  typedef std::uniform_int_distribution<long> type;
};
template <> struct uniform_distribution<unsigned short> {
  typedef std::uniform_int_distribution<unsigned short> type;
};
template <> struct uniform_distribution<unsigned int> {
  typedef std::uniform_int_distribution<unsigned int> type;
};
template <> struct uniform_distribution<unsigned long> {
  typedef std::uniform_int_distribution<unsigned long> type;
};
} // namespace traits

template <typename T>
using uniform_distribution = typename traits::uniform_distribution<T>::type;

/** 一様乱数で埋める */
template <class InputIterator, typename T>
void fill_uniform(InputIterator begin, InputIterator end, T lower, T upper) {
  static_assert(std::is_same<iterator_v<InputIterator>, T>::value == true,
                "Type of lower/upper is not consistent with iterator");
  auto dist = uniform_distribution<T>(lower, upper);
  fill(begin, end, dist);
}

/** 配列からランダムにN個選ぶ */
template <class RandomAccessItrator,
          typename T = iterator_v<RandomAccessItrator> >
inline std::vector<T> select_n(RandomAccessItrator begin,
                               RandomAccessItrator end, size_t n) {
  size_t M = end - begin;
  UTILITY_ASSERT_CHECK(M >= n, "size is too small");
  if (M == n)
    return std::vector<T>(begin, end);
  std::vector<size_t> idx(n);
  fill_uniform(idx.begin(), idx.end(), (size_t)0, M - 1);
  std::vector<T> result;
  result.reserve(n);
  for (size_t i : idx) {
    result.push_back(begin[i]);
  }
  return result;
}

} // namespace random
} // namespace utility
