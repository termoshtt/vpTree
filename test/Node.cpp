
#include "../Node.hpp"
#include "../iterator.hpp"
#include "../output.hpp"

#include "utility/test2.hpp"

namespace U = utility;
namespace vp = U::tree::vp;

typedef unsigned long Index;
typedef vp::Node<Index> Node;
typedef vp::LeafNode<Index> LeafNode;
typedef vp::pNode<Index> pNode;

Node *test_tree() {
  const unsigned short m = 2;
  const unsigned short l = 3;
  Node *a = new Node(m, 0);
  auto *b = a->new_Node(m, 1);
  auto *c = a->new_Node(m, 2);
  auto *b1 = b->new_LeafNode(l);
  b1->add(3);
  b1->add(4);
  auto *b2 = b->new_LeafNode(l);
  b2->add(5);
  auto *c1 = c->new_LeafNode(l);
  c1->add(6);
  c1->add(7);
  c1->add(8);
  auto *c2 = c->new_LeafNode(l);
  c2->add(9);
  vp::show_dot(a, "tree.dot");
  return a;
}

void test_get_covering_node() {
  Node node(5, 0);
  node.mu = { 0.2, 0.3 };
  TEST2_CHECK_EQUAL_INT(node.get_covering_node(0.0), 0);
  TEST2_CHECK_EQUAL_INT(node.get_covering_node(0.25), 1);
  TEST2_CHECK_EQUAL_INT(node.get_covering_node(0.35), 2);
}

void test_iterator() {
  pNode p(test_tree());
  auto b = vp::begin(p.get());
  auto e = vp::end(p.get());
  TEST2_CHECK_EQUAL_INT(*b++, 0);
  TEST2_CHECK_EQUAL_INT(*b++, 1);
  TEST2_CHECK_EQUAL_INT(*b++, 3);
  TEST2_CHECK_EQUAL_INT(*b++, 4);
  TEST2_CHECK_EQUAL_INT(*b++, 5);
  TEST2_CHECK_EQUAL_INT(*b++, 2);
  TEST2_CHECK_EQUAL_INT(*b++, 6);
  TEST2_CHECK_EQUAL_INT(*b++, 7);
  TEST2_CHECK_EQUAL_INT(*b++, 8);
  TEST2_CHECK_EQUAL_INT(*b++, 9);
  TEST2_CHECK(b == e);
}

int main(int argc, char const *argv[]) {
  test_get_covering_node();
  test_iterator();
  return 0;
}
