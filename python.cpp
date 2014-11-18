
#include "boost/numpy.hpp"

namespace p = boost::python;
namespace np = boost::numpy;

BOOST_PYTHON_MODULE(vptree) {
  Py_Initialize();
  np::initialize();
}
