#ifndef __lcpp_laswp_h
#define __lcpp_laswp_h

#include "matrix.h"

namespace LCPP {

    /// <summary>
    /// performs a series of row interchanges on the matrix A.
    /// One row interchange is initiated for each of the rows of A in the pivots
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class LASWP {
    public:

        LASWP() {}

        void operator() (NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<int> pivots);

    private:

    };

    template <typename T>
    void LASWP<T>::operator() (NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<int> pivots) {
        if (pivots.isEmpty())
            return;
        const int* b = pivots.cbegin(), * e = pivots.cend();
        int n = A.getNrows();
        for (int i = 0; i < n; ++i) {

        }
        while (b != e) {
            int ip = pivots[ix];
            if (ip != i) {
                A.row(i).swap(A.row(ip));
            }
        }
        for (int i = i1, ix=ix0; i <= i2; i += inc, ix+=incx) {
            int ip = pivots[ix];
            if (ip != i) {
                A.row(i).swap(A.row(ip));
            }
        }
    }
}

#endif
