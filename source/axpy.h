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


        void operator()(T a, const NUMCPP::Sequence<T>& X, NUMCPP::Sequence<T>& Y) {
            int incx = X.increment(), incy = Y.increment();
            apply(Y.length(), a, X.begin(), X.increment(), Y.begin(), Y.increment());
        }

    private:

        void apply(int n, T a, const T* X, int incx, T* Y, int incy);

    };

    template <typename T>
    void AXPY<T>::apply(int n, T a, const T* x, int incx, T* y, int incy) {
        if (n == 0)
            return;
        if (a == 0)
            return;
        const T* const e = x + n * incx;
        while (e != x) {
            *y += a * (*x);
            y += incy;
            x += incx;
        }
    }

}

#endif

