// lcpp.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <chrono>
#include <ratio>
#include <ctime>
#include <numeric>
#include <cmath>
#include "sequence.h"
#include "matrix.h"
#include "axpy.h"
#include "scal.h"
#include "copy.h"
#include "swap.h"
#include "dot.h"
#include "rot.h"
#include "nrm2.h"

using namespace NUMCPP;
using namespace LCPP;

int main()
{
    size_t m = 10, n = 15, k = 50;
    Matrix<double> Q(m, n, [](size_t r, size_t c) { return (double)((r + 1) + 100 * (c + 1)); });
    Matrix<double> P(5, 5);
    P = Q;
    Q = transpose(P.all());

    DataBlock<double> V(k);
    V.rand();

    DataBlock<double> W(k);
    W.rand();

    Sequence<double> w = W.all(), v = V.all();
    SWAP<double> swap;
    DOT<double, double> dot;
    NRM2<double, double> nrm2;
    ROT<double> rot;

    double q1 = 0, q2 = 0;
    const auto start1 = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < 10000000; ++i) {
        swap(k, v.begin(),1, w.begin(),1);
        q1 = dot(k, v.begin(),1, w.begin(),1);
        q1 = nrm2(k, w.begin());
    }
    const auto end1 = std::chrono::steady_clock::now();
    const auto int_ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    const auto start2 = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < 10000000; ++i) {
        //       Sequence<double> rw = w.reverse();
        swap(v, w);
        q2 = dot(v, w);
        q2 = nrm2(w);
    }
    const auto end2 = std::chrono::steady_clock::now();
    const auto int_ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);

    std::cout << int_ms1.count() << '\t' << int_ms2.count() << std::endl;
    std::cout << q1 << '\t' << q2 << std::endl;

}

