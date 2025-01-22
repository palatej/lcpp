#include "TestBlas.h"
#include <iostream>
#include <chrono>
#include <ratio>
#include <ctime>
#include <numeric>
#include <cmath>

#include "copy.h"
#include "axpy.h"
#include "asum.h"
#include "swap.h"

using namespace NUMCPP;
using namespace LCPP;

void TestBlas::test1(int m, int q) {
    DataBlock<double> A(m);
    A.rand();
    DataBlock<double> B(m);
    B.rand();

    DataBlock<double> V = A, W = B;

    Sequence<double> w = W.all(), v = V.all();
    COPY<double> copy;
    AXPY<double> axpy;
    SWAP<double> swap;
    ASUM<double>asum;
    const auto start = std::chrono::steady_clock::now();
    double s = 0;
    for (unsigned i = 0; i < q; ++i) {
        copy(A.all(), v);
        copy(B.all(), w);
        swap(v, w);
        axpy(1.23, v, w);
        //axpy(-1.23, v.reverse(), w);
        s += asum(w);
    }
    const auto end = std::chrono::steady_clock::now();
    const auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << int_ms.count() << '\t' << s << std::endl;
}