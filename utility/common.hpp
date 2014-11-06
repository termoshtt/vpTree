#pragma once
#include <iterator>
#include <boost/range/value_type.hpp>
namespace utility {
template <typename Iterator>
using iterator_v = typename std::iterator_traits<Iterator>::value_type;
template <typename Iterator>
using iterator_d = typename std::iterator_traits<Iterator>::difference_type;
template <typename Range>
using range_v = typename boost::range_value<Range>::value;
} // namespace utility
