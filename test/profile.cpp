
#include "../Tree.hpp"
#include "utility/random.hpp"
#include "utility/log.hpp"
#include "utility/output.hpp"

#include <chrono>
#include <iostream>
#include <iomanip>

namespace U = utility;
namespace vp = U::tree::vp;
namespace C = std::chrono;

typedef unsigned long Index;
typedef vp::Node<Index> Node;
typedef vp::LeafNode<Index> LeafNode;
typedef vp::pNode<Index> pNode;
typedef std::array<double, 1> Point;
typedef vp::Tree<Point, Index> Tree;

double construct(Index N, unsigned short M, unsigned short L,
                 unsigned long max_linear, unsigned int n_trial,
                 unsigned int m_random) {
  Tree tree(M, L, max_linear, n_trial, m_random);

  auto s = U::random::seed();
  std::mt19937 gen(s);
  U::random::uniform_distribution<double> dist(-1, 1);

  std::vector<Point> points;
  for (Index n = 0; n < N; ++n) {
    points.push_back({ dist(gen) });
  }
  auto start = C::system_clock::now();
  for (auto &p : points) {
    tree.insert(p);
  }
  auto finish = C::system_clock::now();
  return C::duration_cast<C::microseconds>(finish - start).count();
}

double search(Index N, float tau, unsigned short M, unsigned short L,
              unsigned long max_linear, unsigned int n_trial,
              unsigned int m_random) {
  Tree tree(M, L, max_linear, n_trial, m_random);

  auto s = U::random::seed();
  std::mt19937 gen(s);
  U::random::uniform_distribution<double> dist(-1, 1);
  std::vector<Point> points;
  for (Index n = 0; n < N; ++n) {
    tree.insert({ dist(gen) });
    points.push_back({ dist(gen) });
  }

  auto start = C::system_clock::now();
  for (auto &p : points) {
    tree.get_nearest(p, tau);
  }
  auto finish = C::system_clock::now();
  return C::duration_cast<C::microseconds>(finish - start).count() / (double)N;
}

int main(int argc, char const *argv[]) {
  unsigned short M = 3, L = 5;
  unsigned int n_trial = 10;
  unsigned int m_random = 10;
  U::log("N construct search(t=0.1) search(t=1) search(linear)", std::cout);
  std::cout << std::scientific << std::setprecision(7);
  for (Index N = 2 << 5; N <= 2 << 12; N *= 2) {
    double s_a = search(N, 1, M, L, M * L, n_trial, m_random);
    double s = search(N, 0.1, M, L, M * L, n_trial, m_random);
    double s_l = search(N, 1, M, L, N, n_trial, m_random);
    double s_c = construct(N, M, L, M * L, n_trial, m_random);
    U::csv_wrap(std::cout, " ") << N << s_c << s << s_a << s_l;
  }
  return 0;
}
