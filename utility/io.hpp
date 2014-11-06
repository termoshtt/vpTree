#pragma once

#include "log.hpp"

#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

namespace utility {

/**
 * I/O for one-dimensional data
 */
namespace io {

template <typename T> class ASCII;
template <typename T> class BINARY;

/** load data from file
 * \tparam T typename of data (double, float, int)
 * \tparam Format ASCII or BINARY
 *
 * \return size of data
 * \param filename in which data is saved
 * \param data loaded data will be placed here
 */
template <typename T, template <class> class Format = ASCII>
int load(std::string filename, T *data) {
  return Format<T>::load(filename, data);
}

/** load data from file
 * \tparam T typename of data (double, float, int)
 * \tparam Format ASCII or BINARY
 *
 * \return size of data
 * \param filename in which data is saved
 * \param data loaded data will be placed here
 */
template <typename T, template <class> class Format = ASCII>
void load(std::string filename, int N, T *data) {
  Format<T>::load(filename, N, data);
}

/** load data from file
 * \tparam T typename of data (double, float, int)
 * \tparam Format ASCII or BINARY
 *
 * \return vector containing data
 * \param filename in which data is saved
 */
template <typename T, template <class> class Format = ASCII>
std::vector<T> load(std::string filename) {
  return Format<T>::load(filename);
}

/** dump data into file
 * \tparam T typename of data (double, float, int)
 * \tparam Format ASCII or BINARY
 *
 * \param filename in which data will be saved
 * \param N size of data
 * \param data to be saved
 */
template <class InputIterator, template <class> class Format = ASCII>
void dump(InputIterator begin, InputIterator end, std::string filename) {
  typedef typename std::iterator_traits<InputIterator>::value_type T;
  Format<T>::dump(begin, end, filename);
}

/** dump data into file
 * \tparam T typename of data (double, float, int)
 * \tparam Format ASCII or BINARY
 *
 * \param filename in which data will be saved
 * \param N size of data
 * \param data to be saved
 */
template <class InputIterator, template <class> class Format = ASCII>
void show(InputIterator begin, InputIterator end, std::ostream &ost) {
  typedef typename std::iterator_traits<InputIterator>::value_type T;
  Format<T>::show(begin, end, ost);
}

template <class InputIterator, template <class> class Format = ASCII>
void show(InputIterator begin, InputIterator end, InputIterator b2,
          std::ostream &ost, std::string comma = " ") {
  typedef typename std::iterator_traits<InputIterator>::value_type T;
  Format<T>::show(begin, end, b2, ost, comma);
}

/*
 * implementations
 * 上述の関数を部分特殊化できないので(C++の制限)、
 * クラスに分けて特殊化する
 */

/**
 * \class ASCII io.hpp "utility/io.hpp"
 *
 * I/O for ascii format
 */
template <typename T> class ASCII {

public:
  static void load(std::string filename, int N, T *data);
  static int load(std::string filename, T *data);
  static std::vector<T> load(std::string filename);

  static void set_format(std::ostream &ost) {}

  template <class InputIterator>
  static void show(InputIterator begin, InputIterator end, std::ostream &ost) {
    set_format(ost);
    while (begin != end) {
      ost << *begin++ << '\n';
    }
    ost << std::flush;
  }

  template <class InputIterator>
  static void show(InputIterator begin, InputIterator end, InputIterator b2,
                   std::ostream &ost, std::string comma) {
    set_format(ost);
    while (begin != end) {
      ost << *begin++ << comma << *b2++ << '\n';
    }
    ost << std::flush;
  }

  template <class InputIterator>
  static void dump(InputIterator begin, InputIterator end,
                   std::string filename) {
    std::ofstream ofs(filename);
    if (ofs.fail()) {
      char msg[128];
      sprintf(msg, "file %s cannot be opened", filename.c_str());
      throw std::runtime_error(msg);
    }
    show(begin, end, ofs);
  }
};

template <> inline void ASCII<float>::set_format(std::ostream &ost) {
  ost << std::scientific << std::setprecision(8);
}
template <> inline void ASCII<double>::set_format(std::ostream &ost) {
  ost << std::scientific << std::setprecision(15);
}

template <typename T> std::vector<T> ASCII<T>::load(std::string filename) {
  std::ifstream ifs(filename);
  if (ifs.fail()) {
    char msg[128];
    sprintf(msg, "file %s does not found", filename.c_str());
    throw std::runtime_error(msg);
  }

  std::vector<T> data;
  std::string line;
  while (std::getline(ifs, line)) {
    T val;
    try {
      val = boost::lexical_cast<T>(line);
    }
    catch (...) {
      char msg[128];
      sprintf(msg, "cannot convert: '%s'", line.c_str());
      log(msg, std::cerr);
      continue;
    }
    data.push_back(val);
  }
  return data;
}

template <typename T> int ASCII<T>::load(std::string filename, T *data) {
  std::vector<T> data_ = ASCII<T>::load(filename);
  std::copy(data_.begin(), data_.end(), data);
  return data_.size();
}

template <typename T>
void ASCII<T>::load(std::string filename, int N, T *data) {
  std::vector<T> data_ = ASCII<T>::load(filename);
  if ((int)data_.size() == N) {
    std::copy(data_.begin(), data_.end(), data);
  } else if ((int)data_.size() > N) {
    log("Strip surplus data", std::cerr);
    std::copy(data_.begin(), data_.begin() + N, data);
  } else {
    char msg[128];
    sprintf(msg, "Cannot enough data from file '%s'", filename.c_str());
    throw std::runtime_error(msg);
  }
}

/**
 * \class BINARY io.hpp "utility/io.hpp"
 *
 * I/O for binary format
 */
template <typename T> class BINARY {
public:
  static int load(std::string filename, T *data);
  static std::vector<T> load(std::string filename);
  static void dump(std::string filename, int N, T *data);
};

} // namespace io
} // namespace utility
