#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

namespace vpTree {

/** vpTreeの基底例外 */
class Error : public std::logic_error {
public:
  Error(std::string message) : std::logic_error(message) {}
};

inline void check(bool st, std::string message) {
  if (!st)
    throw Error(message);
}

} // namespace vpTree
