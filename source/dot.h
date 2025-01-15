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
            return apply(n, X, 1, Y, 1);
        }

        S operator()(NUMCPP::Sequence<T>& X, NUMCPP::Sequence<T>& Y) {
            int incx = X.increment(), incy = Y.increment(), n = Y.length();
            if (incx == 1 && incy == 1)
                return apply(n, X.begin(), Y.begin());
            else
                return apply(n, X.begin(), incx, Y.begin(), incy);
        }

    private:

        S apply(int n, const T* X, int incx, const T* Y, int incy);
        S apply(int n, const T* X, const T* Y);

    };

    template <typename T, typename S>
    S DOT<T, S>::apply(int n, const T* X, const T* Y) {
        S zero = NUMCPP::CONSTANTS<S>::zero;
        if (n == 0)
            return zero;
        S dot = zero;
        const T* y = Y;
        const T* x = X;
        const T* const e = X + n;
        while (x != e) {
            dot += (*y++) * (*x++);
        }
        return dot;
    }

    template <typename T, typename S>
    S DOT<T, S>::apply(int n, const T* X, int incx, const T* Y, int incy) {
        S zero = NUMCPP::CONSTANTS<S>::zero;
        if (n == 0)
            return zero;
        S dot = zero;
        const T* y = Y;
        const T* x = X;
        const T* const e = X + incx * n;
        while (x != e) {
            dot += (*y) * (*x);
            y += incy;
            x += incx;
        }
        return dot;
    }
}
#endif
