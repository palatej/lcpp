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
#include "gemm.h"

using namespace NUMCPP;
using namespace LCPP;

int main()
{
    try {
        int m = 10, n = 15, k = 5;
        Matrix<double> Q(m, n, [](int r, int c) { return (double)((r + 1) + 100 * (c + 1)); });
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
        GEMM<double> gemm;

        double q1 = 0, q2 = 0;
        const auto start1 = std::chrono::steady_clock::now();
        for (unsigned i = 0; i < 1000000; ++i) {
            swap(k, v.begin(), 1, w.begin(), 1);
            q1 = dot(k, v.begin(), 1, w.begin(), 1);
            q1 = nrm2(k, w.begin());
        }
        const auto end1 = std::chrono::steady_clock::now();
        const auto int_ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
        const auto start2 = std::chrono::steady_clock::now();
        for (unsigned i = 0; i < 1000000; ++i) {
            //       Sequence<double> rw = w.reverse();
            swap(v, w);
            q2 = dot(v, w);
            q2 = nrm2(w);
        }
        const auto end2 = std::chrono::steady_clock::now();
        const auto int_ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);

        std::cout << int_ms1.count() << '\t' << int_ms2.count() << std::endl;
        std::cout << q1 << '\t' << q2 << std::endl;

        const auto start3 = std::chrono::steady_clock::now();
        for (unsigned i = 0; i < 1000000; ++i) {
            Matrix<double> A(m, k, [](int r, int c) { return (double)((r + 1) + 100 * (c + 1)); });
            Matrix<double> B(k, n, [](int r, int c) { return (double)(25 * (r + 1) - 25 * (c + 1)); });
            Matrix<double> C(m, n, [](int r, int c) { return (double)((r + 1) + (c + 1)); });


            gemm(false, false, 2, A, B, -1, C);
            //        std::cout << C << std::endl;

            C = Matrix<double>(m, n, [](int r, int c) { return (double)((r + 1) + (c + 1)); });
            B = transpose(B.all());
            gemm(false, true, 2, A, B, -1, C);
            //        std::cout << C << std::endl;

            B = Matrix<double>(k, n, [](int r, int c) { return (double)(25 * (r + 1) - 25 * (c + 1)); });
            C = Matrix<double>(m, n, [](int r, int c) { return (double)((r + 1) + (c + 1)); });
            A = transpose(A.all());
            gemm(true, false, 2, A, B, -1, C);
            //       std::cout << C << std::endl;

            A = Matrix<double>(m, k, [](int r, int c) { return (double)((r + 1) + 100 * (c + 1)); });
            B = Matrix<double>(k, n, [](int r, int c) { return (double)(25 * (r + 1) - 25 * (c + 1)); });
            C = Matrix<double>(m, n, [](int r, int c) { return (double)((r + 1) + (c + 1)); });
            A = transpose(A.all());
            B = transpose(B.all());
            gemm(true, true, 2, A, B, -1, C);
            //        std::cout <<C << std::endl;
        }
        const auto end3 = std::chrono::steady_clock::now();
        const auto int_ms3 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - start3);

        std::cout << int_ms3.count() << std::endl;
    }
    catch (const std::exception& err) {
        std::cout << err.what();
    }
}

