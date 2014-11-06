
#include "../Omega.hpp"
#include "utility/test2.hpp"

#include <array>

namespace vp = utility::tree::vp;

typedef std::array<int, 1> Point;
typedef unsigned int Index;
typedef vp::Omega<Point, Index> Omega;

int main(int argc, char const *argv[]) {
  Omega points;
  TEST2_CHECK_EQUAL_INT(points.insert({ 1 }), 0);
  TEST2_CHECK_EQUAL_INT(points.insert({ 2 }), 1);
  TEST2_CHECK_EQUAL_INT(points.insert({ 5 }), 2);
  points.erase(1);
  TEST2_CHECK_EQUAL_INT(points.insert({ 3 }), 1);
  TEST2_CHECK_EQUAL_INT(points.insert({ 4 }), 3);

  TEST2_CHECK_EQUAL_REAL_TH(points.d(0, 1), 2.0 / (1 + 2), 1e-7);
  points.erase(2);

  try {
    points.d(0, 2);
  }
  catch (utility::AssertionFailed &e) {
    std::cout << e.what() << std::endl;
  } // deleted index

  return 0;
}
