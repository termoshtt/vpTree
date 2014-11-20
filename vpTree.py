#!/usr/bin/env python
# -*- coding: utf-8 -*-


import numpy as np
import vpTree


def test_d():
    x = np.array((1., 2.))  # double
    y = np.array((2., 3.))
    print(x.dtype)
    print("for float:   " + str(vpTree.d(x, y)))

    x = np.array((1, 2))  # int
    y = np.array((2, 3))
    print(x.dtype)
    print("for integer: " + str(vpTree.d(x, y)))
    print("True:        " + str(np.sqrt(2)/(1+np.sqrt(2))))


def test_Tree():
    N = 100
    M = 2
    L = 3
    max_lin = 3
    n_trial = 10
    m_random = 10
    t = vpTree.Tree(M, L, max_lin, n_trial, m_random)

    print(t.empty())

    for i in range(N):
        arr = np.array((1.0*i,1.0*i))
        idx = t.insert(arr)
        a = t.get(idx)
        print((arr == a).all())

if __name__ == '__main__':
    test_d()
    test_Tree()
