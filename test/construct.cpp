
#include "../construct.hpp"
#include "../output.hpp"
#include "../insert.hpp"

#include "utility/test2.hpp"
#include "utility/log.hpp"

namespace U = utility;
namespace vp = vpTree;

typedef std::array<double, 1> Point;
typedef unsigned int Index;
typedef vp::Omega<Point, Index> Omega;
typedef vp::Node<Index> Node;
typedef vp::LeafNode<Index> LeafNode;

void test_select_vp() {
  Omega points;
  std::vector<Index> indices;
  indices.push_back(points.insert({ 0 }));  // 0
  indices.push_back(points.insert({ -1 })); // 1
  indices.push_back(points.insert({ 1 }));  // 2
  indices.push_back(points.insert({ 4 }));  // 3
  indices.push_back(points.insert({ 2 }));  // 4
  Index idx = vp::select_vp(points, indices, indices.size(), indices.size());
  TEST2_CHECK_EQUAL_INT(idx, 0);
}

/** muの設定の確認
 *
 * vpからの距離に応じて点を入れる部分が正しく決定されているか
 */
void test_tree(Omega &omega, Node *n) {
  if (!n->children.empty()) {
    for (unsigned short m = 0; m < n->m; ++m) {
      Node *cn = n->children[m].get();
      auto b = vp::begin(cn);
      auto e = vp::end(cn);
      while (b != e) {
        auto d = omega.d(n->vp, *b++);
        if (m < n->m - 1)
          TEST2_CHECK(d < n->mu[m]);
        if (m > 0)
          TEST2_CHECK(d >= n->mu[m - 1]);
      }
      test_tree(omega, cn);
    }
  } else {
    for (unsigned short m = 0; m < n->m; ++m) {
      LeafNode *cn = n->leaves[m].get();
      auto b = cn->points.begin();
      auto e = cn->points.end();
      while (b != e) {
        auto d = omega.d(n->vp, *b++);
        if (m < n->m - 1)
          TEST2_CHECK(d < n->mu[m]);
        if (m > 0)
          TEST2_CHECK(d >= n->mu[m - 1]);
      }
    }
  }
}

void test_construct() {
  std::vector<Point> points;
  for (int i = 0; i < 30; ++i) {
    points.push_back({(double)i });
  }
  Omega omega;
  auto indices = vp::register_points(omega, points);

  unsigned short M = 3;
  unsigned short L = 5;
  unsigned int n_trial = indices.size();
  unsigned int m_random = indices.size();
  auto root =
      vp::pNode<Index>(vp::construct(omega, indices, M, L, n_trial, m_random));
  vp::show_dot(root.get(), "vp.dot");
  auto gathered = vp::gather(root.get());
  TEST2_CHECK_EQUAL_INT(gathered.size(), 30);
  test_tree(omega, root.get());

  for (int i = 0; i < 10; ++i) {
    vp::insert(omega, root, Point({ -1.0 * i }), M, L, n_trial, m_random);
    char filename[1024];
    sprintf(filename, "vp%02d.dot", i);
    vp::show_dot(root.get(), filename);
  }
  test_tree(omega, root.get());
}

int main(int argc, char const *argv[]) {
  test_select_vp();
  test_construct();
  return 0;
}
