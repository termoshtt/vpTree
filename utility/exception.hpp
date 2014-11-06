#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

namespace utility {

struct AlgorithmException : public std::runtime_error {
  const std::string type;
  AlgorithmException(const std::string &type, const std::string &msg)
      : std::runtime_error(msg), type(type) {}
  ~AlgorithmException() throw() {}
};

struct AssertionFailed : public std::runtime_error {
  std::string filename;
  std::string funcname;
  int line;
  AssertionFailed(const std::string &msg, const std::string &filename,
                  const std::string &funcname, int line)
      : std::runtime_error(msg), filename(filename), funcname(funcname),
        line(line) {}
  virtual const char *what() const throw() {
    std::ostringstream oss;
    oss << filename << ":" << line << ":" << funcname << " ";
    oss << std::runtime_error::what();
    return oss.str().c_str();
  }
  ~AssertionFailed() throw() {}
};

} // namespace utility
