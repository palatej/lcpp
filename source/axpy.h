#ifndef __lcpp_axpy_h
#define __lcpp_axpy_h

#include "sequence.h"

namespace LCPP {
    template <typename T>
    class AXPY {
    public:
        AXPY() {}

        void operator()(int n, T a, const T* X, int incx, T* Y, int incy) {
            apply(n, a, X, incx, Y, incy);
        }

        void operator()(int n, T a, const T* X, T* Y) {
            apply(n, a, X, 1, Y, 1);
        }


        void operator()(T a,  NUMCPP::Sequence<T> X, NUMCPP::Sequence<T> Y) {
            apply(Y.length(), a, X.begin(), X.increment(), Y.begin(), Y.increment());
        }

    private:

        void apply(int n, T a, const T* X, int incx, T* Y, int incy);

    };

    template <typename T>
    void AXPY<T>::apply(int n, T a, const T* x, int incx, T* y, int incy) {
        if (a == 0)
            return;
        int imax = incx * n;
        for (int i=0, j=0; i!= imax; i+=incx, j+=incy)
            y[j] += a * x[i];
    }

}

#endif

