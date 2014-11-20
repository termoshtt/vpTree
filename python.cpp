
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
  auto x_type = x.get_dtype();
  auto y_type = y.get_dtype();
  if (x_type != y_type)
    throw std::runtime_error("Type of arguments mismatch");
  double ed = 0;
  if (x_type == np::dtype::get_builtin<double>()) {
    double *x0 = reinterpret_cast<double *>(x.get_data());
    double *y0 = reinterpret_cast<double *>(y.get_data());
    ed = utility::euclid_distance(x0, x0 + N, y0);
  } else if (x_type == np::dtype::get_builtin<long long>()) {
    long long *x0 = reinterpret_cast<long long *>(x.get_data());
    long long *y0 = reinterpret_cast<long long *>(y.get_data());
    ed = utility::euclid_distance(x0, x0 + N, y0);
  } else {
    throw std::runtime_error("Only int64 and float64 are supported");
  }
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
  p::tuple get_nearest(const Point &x, float tau) const {
    auto near = Inherited::get_nearest(x, tau);
    return p::make_tuple(std::get<0>(near), std::get<1>(near));
  }
};

BOOST_PYTHON_MODULE(vpTree) {
  Py_Initialize();
  np::initialize();

  p::def("d", &vpTree::d<np::ndarray>);

  auto t = p::class_<Tree, boost::noncopyable>(
      "Tree", p::init<unsigned short, unsigned short, unsigned long,
                      unsigned int, unsigned int>());
  t.def("empty", &Tree::empty);
  t.def("insert", &Tree::insert);
  t.def("get", &Tree::get);
  t.def("tree_constructed", &Tree::tree_constructed);
  t.def("get_nearest", &Tree::get_nearest);
}
