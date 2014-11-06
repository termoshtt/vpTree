#pragma once

/*! \file output.hpp
 *  \brief データ出力を行うutility関数群 */

#include <iostream>
#include <iomanip>
#include <iterator>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

namespace utility {

template <typename T> inline std::ostream &common_iomanip(std::ostream &ost) {
  return ost;
}

template <> inline std::ostream &common_iomanip<double>(std::ostream &ost) {
  ost << std::setprecision(15) << std::scientific;
  return ost;
}

/*!
 * 配列に入っているデータを出力する
 *
 * \param N データの数
 * \param u データの配列
 * \param ost データの出力先
 */
template <typename T>
inline void show(const int N, const T *u, std::ostream &ost = std::cout,
                 std::string delimitor = "\n") {
  ost << common_iomanip<T>;
  std::ostream_iterator<T> oit(ost, delimitor.c_str());
  std::copy(u, u + N, oit);
  ost << std::flush;
}

/*!
 * 複数出力したいとき
 *
 * \param N データの数
 * \param u1 データの配列
 * \param u2 データの配列
 * \param ost データの出力先
 */
template <typename T>
inline void show(int N, const T *u1, const T *u2, std::ostream &ost = std::cout,
                 std::string separator = " ", std::string delimitor = "\n") {
  ost << common_iomanip<T>;
  for (int i = 0; i < N; ++i) {
    ost << u1[i] << separator << u2[i] << delimitor;
  }
  ost << std::flush;
}

inline void show(const int N, const double *u, std::string filename) {
  std::ofstream ofs(filename);
  show(N, u, ofs);
}

inline void show(const int N, const double *u1, const double *u2,
                 std::string filename) {
  std::ofstream ofs(filename);
  show(N, u1, u2, ofs);
}

class csv_wrap {
  std::ostream &ost;
  std::string comma, endl;
  bool is_first;
  bool is_flush;

public:
  csv_wrap(std::ostream &ost, std::string comma, bool is_flush = true,
           std::string endl = "\n")
      : ost(ost), comma(comma), endl(endl), is_first(true), is_flush(is_flush) {
  }
  ~csv_wrap() {
    if (is_flush) {
      ost << endl << std::flush;
    } else {
      ost << endl;
    }
  }
  template <typename T> csv_wrap &operator<<(const T &val) {
    if (is_first) {
      ost << val;
      is_first = false;
    } else {
      ost << comma << val;
    }
    return *this;
  }
  csv_wrap &operator<<(std::ostream &(*pf)(std::ostream &)) {
    pf(ost);
    return *this;
  }
};

} // namespace utility
