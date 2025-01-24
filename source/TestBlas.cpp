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
#include "matrix.h"
#include "gemv.h"

using namespace NUMCPP;
using namespace LCPP;

void TestBlas::test1(int m, int q) {
    DataBlock<double> A(m);
    A.rand();
    DataBlock<double> B(m);
    B.rand();

    std::vector<double> Q(m);
    std::iota(Q.begin(), Q.end(), 1);

    DataBlock<double> V = A, W = B;

    Sequence<double> w = W.all(), v = V.all();
    SWAP<double> swap;
    const auto start = std::chrono::steady_clock::now();
    double s = 0;
    for (unsigned i = 0; i < q; ++i) {
        //copy(A.all(), v);
        //copy(B.all(), w);
        v.copy(A.all());
        w.copy(B.all());
        //swap(v, w);
        //axpy(1.23, v, w);
        //axpy(-1.23, v.reverse(), w);
        w.addAY(1.23, v);
        w.addAY(-1.23, v.reverse());
        s += w.asum();
        s -= w.sum2();
        //s -= ::Sum(Q);
        
       // s += w.sum([](double x) {return abs(x); });
    }
    const auto end = std::chrono::steady_clock::now();
//    swap(v, w);
    const auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << int_ms.count() << '\t' << s << std::endl;
}

void TestBlas::test2(int m, int n, int q) {
    COPY<double> copy;
    GEMV<double> gemv;
    DataBlock<double> X(n, [](int i) {return i + 1; });
    DataBlock<double> Y(m, [](int i) {return i * 5; });
    Matrix<double>A(m, n, [](int i, int j) {return i + j; });
    const auto start = std::chrono::steady_clock::now();
    DataBlock<double> Z = Y;
    Sequence<double> y = Y.all();
    for (unsigned i = 0; i < q; ++i) {
        Y.all().copy(Z.all());
        gemv(false, 2, A.all(), X.all(), 10, y);
    }
    const auto end = std::chrono::steady_clock::now();
    const auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << int_ms.count() << std::endl <<Z<<std::endl<< A << std::endl << X << std::endl << y << std::endl;

}