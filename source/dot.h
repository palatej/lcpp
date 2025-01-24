#ifndef __lcpp_dot_h
#define __lcpp_dot_h

#include "sequence.h"

namespace LCPP {

    template <typename T, typename S>
    class DOT {
    public:
        DOT() {}

        S operator()(int n, const T* X, int incx, const T* Y, int incy) {
            return apply(n, X, incx, Y, incy);

        }

        S operator()(int n, const T* X, const T* Y) {
            return apply(n, X, Y);
        }

        S operator()(NUMCPP::Sequence<T> X, NUMCPP::Sequence<T> Y) {
            int incx = X.increment(), incy = Y.increment(), n = Y.length();
            if (incx == 1 && incy == 1)
                return apply(n, &*X.begin(), &*Y.begin());
            else
                return apply(n, &*X.begin(), incx, &*Y.begin(), incy);
        }

    private:

        S apply(int n, const T* X, int incx, const T* Y, int incy);
        S apply(int n, const T* X, const T* Y);

    };

    template <typename T, typename S>
    S DOT<T, S>::apply(int n, const T* X, const T* Y) {
        S dot = NUMCPP::CONSTANTS<S>::zero;
        for (int i = 0; i < n; ++i)
            dot += X[i] * Y[i];
        return dot;
    }

    template <typename T, typename S>
    S DOT<T, S>::apply(int n, const T* X, int incx, const T* Y, int incy) {
        S dot = NUMCPP::CONSTANTS<S>::zero;
        int imax = incx * n;
        if (incx == incy)
            for (int i = 0; i != imax; i += incx)
                dot += X[i] * Y[i];
        else
            for (int i = 0, j = 0; i != imax; i += incx, j += incy)
                dot += X[i] * Y[j];
        return dot;
    }
}
#endif
