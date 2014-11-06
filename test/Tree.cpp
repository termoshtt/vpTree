
#include "../Tree.hpp"
#include "utility/test2.hpp"
#include "utility/log.hpp"
#include <array>

namespace U = utility;
namespace vp = U::tree::vp;

typedef unsigned long Index;
typedef std::array<double, 1> Point;

typedef vp::Node<Index> Node;
typedef vp::LeafNode<Index> LeafNode;
typedef vp::pNode<Index> pNode;
typedef vp::Tree<Point, Index> Tree;

int main(int argc, char const *argv[]) {
  unsigned int M = 3, L = 5, max_linear = 10, n_trial = 10, m_random = 10;
  Tree tree(M, L, max_linear, n_trial, m_random);
  for (unsigned int i = 0; i < max_linear; ++i) {
    tree.insert({ 1.0 * i });
  }
  TEST2_CHECK(!tree.tree_constructed());
  tree.insert({ max_linear * 1.0 });
  TEST2_CHECK(tree.tree_constructed());
  for (Index idx : tree) {
    U::log("idx", idx, std::cout);
    auto p = tree.get(idx);
    U::log("p", p[0], std::cout);

    Index i;
    float d;
    std::tie(i, d) = tree.get_nearest({ 1.0 * idx }, 0.1);
    TEST2_CHECK_EQUAL_INT(idx, i);
    TEST2_CHECK_EQUAL_REAL(d, 0.0);

    float tau = 0.1;
    std::tie(i, d) = tree.get_nearest({ 1.0 * idx + 0.1 }, tau);
    U::log("i", i, std::cout);
    U::log("d", d, std::cout);
    TEST2_CHECK_EQUAL_INT(idx, i);
    TEST2_CHECK_EQUAL_REAL_TH(d, 0.1 / 1.1, 1e-7);

    tau = 1e-5;
    std::tie(i, d) = tree.get_nearest({ 1.0 * idx + 0.1 }, tau);
    U::log("i", i, std::cout);
    U::log("d", d, std::cout);
    /* these test may fails */
    // TEST2_CHECK_EQUAL_INT(idx, i);
    // TEST2_CHECK_EQUAL_REAL_TH(d, 0.1 / 1.1, 1e-7);
  }
  return 0;
}
