
#include "Tree.hpp"
#include "utility/algorithm.hpp"
#include "boost/numpy.hpp"

namespace p = boost::python;
namespace np = boost::numpy;

namespace vpTree {

inline size_t size(const np::ndarray &arr) {
  size_t nd = arr.get_nd();
  size_t size = 1;
  for (size_t n = 0; n < nd; ++n) {
    size *= arr.shape(n);
  }
  return size;
}

template <>
inline float d<np::ndarray>(const np::ndarray &x, const np::ndarray &y) {
  size_t N = size(x);
  if (size(y) != N)
    throw std::runtime_error("Size mismatch");
  double *x0 = reinterpret_cast<double *>(x.get_data());
  double *y0 = reinterpret_cast<double *>(y.get_data());

  double ed = utility::euclid_distance(x0, x0 + N, y0);
  return ed / (1 + ed);
}

} // namespace vpTree

class Tree : public vpTree::Tree<np::ndarray, unsigned long> {
  typedef np::ndarray Point;
  typedef unsigned long Index;
  typedef vpTree::Tree<Point, Index> Inherited;

public:
  Tree(unsigned short M, unsigned short L, unsigned long max_linear,
       unsigned int n_trial, unsigned int m_random)
      : Inherited(M, L, max_linear, n_trial, m_random) {}
  np::ndarray get(unsigned long i) const { return Inherited::get(i); }
};

BOOST_PYTHON_MODULE(vptree) {
  Py_Initialize();
  np::initialize();

  auto t = p::class_<Tree, boost::noncopyable>(
      "Tree", p::init<unsigned short, unsigned short, unsigned long,
                      unsigned int, unsigned int>());
  t.def("empty", &Tree::empty);
  t.def("insert", &Tree::insert);
  t.def("get", &Tree::get);
  t.def("tree_constructed", &Tree::tree_constructed);
  t.def("get_nearest", &Tree::get_nearest);
}
