#include "Testmat1.h"
#include <iostream>
#include <chrono>
#include <ratio>
#include <ctime>
#include <numeric>
#include <cmath>

#include "gemm.h"
#include "trmm.h"
#include "trsm.h"

using namespace NUMCPP;
using namespace LCPP;

void
TestMatrix1::testGEMM(int m, int n, int k, int q) {
    try {
        GEMM<double> gemm;


        const auto start = std::chrono::steady_clock::now();
        for (unsigned i = 0; i < q; ++i) {

            Matrix<double> A(m, k, [](int r, int c) { return (double)((r + 1) + 100 * (c + 1)); });
            Matrix<double> B(k, n, [](int r, int c) { return (double)(25 * (r + 1) - 25 * (c + 1)); });
            Matrix<double> C(m, n, [](int r, int c) { return (double)((r + 1) + (c + 1)); });

            gemm(false, false, 1, A, B, 1, C);
            //        std::cout << C << std::endl;

            C=Matrix<double>(m, n, [](int r, int c) { return (double)((r + 1) + (c + 1)); });
            FastMatrix<double> b = B.all();
            B = transpose(b);
            gemm(false, true, 1, A, B, 1, C);
            ////        std::cout << C << std::endl;

            B = Matrix<double>(k, n, [](int r, int c) { return (double)(25 * (r + 1) - 25 * (c + 1)); });
            C = Matrix<double>(m, n, [](int r, int c) { return (double)((r + 1) + (c + 1)); });
            FastMatrix<double> a = A.all();
            A = transpose(a);
            gemm(true, false, 1, A, B, 1, C);
            ////       std::cout << C << std::endl;

            A = Matrix<double>(m, k, [](int r, int c) { return (double)((r + 1) + 100 * (c + 1)); });
            B = Matrix<double>(k, n, [](int r, int c) { return (double)(25 * (r + 1) - 25 * (c + 1)); });
            C = Matrix<double>(m, n, [](int r, int c) { return (double)((r + 1) + (c + 1)); });
            a = A.all();
            b = B.all();
            A = transpose(a);
            B = transpose(b);
            gemm(true, true, 1, A, B, 1, C);
            ////        std::cout <<C << std::endl;
        }
        const auto end = std::chrono::steady_clock::now();
        const auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        //std::cout << C << std::endl;
        std::cout << int_ms.count() << std::endl;
    }
    catch (const std::exception& err) {
        std::cout << err.what();
    }
}


void
TestMatrix1::testTRMM() {
	TRMM<double> trmm;
}

