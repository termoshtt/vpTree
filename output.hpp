#pragma once

#include <ostream>

namespace vpTree {

template <typename Index>
inline void show_dot(Node<Index> *n, std::ostream &ost) {
  size_t count = 0;
  ost << "digraph vpTree {\n";
  show_dot_impl(count, n, ost);
  ost << "}" << std::endl;
}

template <typename Index>
inline void show_dot(Node<Index> *n, std::string filename) {
  std::ofstream ofs(filename);
  show_dot(n, ofs);
}

template <typename Index>
inline void show_dot_impl(size_t &count, Node<Index> *n, std::ostream &ost) {
  Index vp = n->vp;
  if (n->children.empty()) {
    for (auto &l : n->leaves) {
      ost << vp << " -> L" << ++count << ";\n";
      for (Index vpl : l->points) {
        ost << "L" << count << " -> " << vpl << ";\n";
      }
    }
  } else {
    for (auto &pn : n->children) {
      Node<Index> *cn = pn.get();
      ost << vp << " -> " << cn->vp << ";\n";
      show_dot_impl(count, cn, ost);
    }
  }
}

} // namespace vpTree
