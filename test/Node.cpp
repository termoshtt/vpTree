
#include "../Node.hpp"
#include "../iterator.hpp"
#include "../output.hpp"

#include "NumTest/Test.hpp"

namespace vp = vpTree;

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

int test_get_covering_node() {
  Node node(5, 0);
  node.mu = { 0.2, 0.3 };
  NumTest::Test t("get_covering_node");
  t.equal_int(node.get_covering_node(0.0), 0);
  t.equal_int(node.get_covering_node(0.25), 1);
  t.equal_int(node.get_covering_node(0.35), 2);
  return t;
}

int test_iterator() {
  pNode p(test_tree());
  auto b = vp::begin(p.get());
  auto e = vp::end(p.get());
  NumTest::Test t("iterator");
  t.equal_int(*b++, 0);
  t.equal_int(*b++, 1);
  t.equal_int(*b++, 3);
  t.equal_int(*b++, 4);
  t.equal_int(*b++, 5);
  t.equal_int(*b++, 2);
  t.equal_int(*b++, 6);
  t.equal_int(*b++, 7);
  t.equal_int(*b++, 8);
  t.equal_int(*b++, 8);
  t.equal_other(b, e) << "check range is fully tested";
  return t;
}

int main(int argc, char const *argv[]) {
  return test_get_covering_node() + test_iterator();
}
