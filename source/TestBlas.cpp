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
    //SWAP<double> swap;
    const auto start = std::chrono::steady_clock::now();
    double s = 0;
    for (int i = 0; i < q; ++i) {
        ////copy(A.all(), v);
        ////copy(B.all(), w);
        //v.copy(A.all());
        //w.copy(B.all());
        ////swap(v, w);
        ////axpy(1.23, v, w);
        ////axpy(-1.23, v.reverse(), w);
        //w.addAY(1.23, v);
        //w.addAY(-1.23, v.reverse());
        //s -= w.sum();
        //w.chs();
        //apply(w, [](double x) {return -x; });
        //s = w.ssq();
        //v.chs();
        //s -= ::Sum(Q);
        
        //w.apply([](double x) {return x * 1.23; });
        //w.apply([](double x) {return x * (1/1.23); });
        w.mul(1.23);
        w.div(1.23);
        //s += w.sum([](double x) {return abs(x); });
    }
    const auto end = std::chrono::steady_clock::now();
    const auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << int_ms.count()  << std::endl;

    double test[] = { 1,-2,3,-4,5,-6 };
    Sequence<double> Test(test, 6);
    std::cout << Test << std::endl;
    std::cout << Test.sum() << std::endl;
    std::cout << Test.asum() << std::endl;
    std::cout << Test.accumulate([](double s, double x) {return s+abs(x); }) << std::endl;
    std::cout << Test.ssq() << std::endl;
    std::cout << Test.dot(Test) << std::endl;
    std::cout << (Test/=5) << std::endl;
    std::cout << (Test *= 5) << std::endl;
    std::cout << (Test += 5) << std::endl;
    std::cout << (Test -= 5) << std::endl;
}

void TestBlas::test2(int m, int n, int q) {
    COPY<double> copy;
    GEMV<double> gemv;
    DataBlock<double> X(n, [](int i) {return i + 1; });
    DataBlock<double> Y(m, [](int i) {return i * 5; });
    Matrix<double>A(m, n);
    A.set([](int i, int j) {return i + j; });
    const auto start = std::chrono::steady_clock::now();
    DataBlock<double> Z = Y;
    Sequence<double> y = Y.all();
    for (int i = 0; i < q; ++i) {
        Y.all().copy(Z.all());
        gemv(false, 2, A.all(), X.all(), 10, y);
    }
    const auto end = std::chrono::steady_clock::now();
    const auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << int_ms.count() << std::endl <<Z<<std::endl<< A << std::endl << X << std::endl << y << std::endl;


}