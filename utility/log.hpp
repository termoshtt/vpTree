#pragma once

#include "assert.hpp"

#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <string>

namespace utility {

template <typename T> inline std::string to_str(T num) {
  std::ostringstream os;
  os << num;
  return os.str();
}

template <typename T, int MAX_BUFF = 1024>
inline std::string to_str_f(const char *format, T num) {
  char str[MAX_BUFF];
  sprintf(str, format, num);
  return std::string(str);
}

inline std::string filename(std::string prefix, std::string extention,
                            int index, int MaxIndex) {
  unsigned int max_size = to_str(MaxIndex).size();
  std::string index_str = to_str(index);
  if (index_str.size() < max_size) {
    int dif_size = max_size - index_str.size();
    for (int t = 0; t < dif_size; ++t) {
      index_str = std::string("0") + index_str;
    }
  }
  UTILITY_ASSERT_CHECK(index_str.size() == max_size, "");
  if (extention[0] != '.') {
    extention = std::string(".") + extention;
  }
  return prefix + index_str + extention;
}

inline void log(const std::string &comment, std::ostream &ost) {
#ifndef LOG_OFF
  time_t now;
  time(&now);
  struct tm *tm_now = localtime(&now);
  char tmp[15];
  strftime(tmp, 15, "%y/%m/%d %H:%M", tm_now);
  ost << "# (" << tmp << ") " << comment << std::endl;
#endif
}

template <typename T>
inline void log(const std::string &comment, T num, std::ostream &ost) {
#ifndef LOG_OFF
  time_t now;
  time(&now);
  struct tm *tm_now = localtime(&now);
  char tmp[15];
  strftime(tmp, 15, "%y/%m/%d %H:%M", tm_now);
  ost << std::setprecision(15) << std::scientific;
  ost << "# (" << tmp << ") " << comment << " = " << num << std::endl;
#endif
}

inline void log(double num, std::ostream &ost) {
#ifndef LOG_OFF
  ost << std::setprecision(15) << std::scientific;
  ost << num << std::endl;
#endif
}

inline void log(int num, std::ostream &ost) {
#ifndef LOG_OFF
  ost << num << std::endl;
#endif
}

template <typename T>
inline void log(const std::string &comment, T num, std::ostream *ost) {
  if (ost) {
    log(comment, num, *ost);
  }
}

template <typename T> inline void log(T val, std::ostream *ost) {
  if (ost) {
    log(val, *ost);
  }
}

} // namespace utility
