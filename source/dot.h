#ifndef __lcpp_dot_h
#define __lcpp_dot_h

#include "sequence.h"

namespace LCPP {

    template <typename T, typename S>
    class DOT {
    public:
        DOT() {}

        S operator()(size_t n, const T* X, size_t incx, const T* Y, size_t incy) {
            return apply(n, X, incx, Y, incy);

        }

        S operator()(size_t n, const T* X, const T* Y) {
            return apply(n, X, 1, Y, 1);
        }

        S operator()(const NUMCPP::Sequence<T>& X, const NUMCPP::Sequence<T>& Y) {
            size_t incx = X.increment(), incy = Y.increment(), n = Y.length();
            return apply(n, X.begin(), incx, Y.begin(), incy);
        }

    private:

        S apply(size_t n, const T* X, size_t incx, const T* Y, size_t incy);

    };


    template <typename T, typename S>
    S DOT<T, S>::apply(size_t n, const T* X, size_t incx, const T* Y, size_t incy) {
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
