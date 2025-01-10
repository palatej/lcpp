#ifndef __lcpp_rot_h
#define __lcpp_rot_h

#include "sequence.h"

namespace LCPP {

    // ROT applies a plane rotation.
    template <typename T>
    class ROT {
    public:

        ROT() {}

        void operator()(size_t n, T* X, T* Y, T c, T s) {
            apply(n, X, 1, Y, 1, c, s);
        }

        void operator()(size_t n, T* X, size_t incx, T* Y, size_t incy, T c, T s) {
            apply(n, X, incx, Y, incy, c, s);
        }

        void operator()(NUMCPP::Sequence<T>& X, NUMCPP::Sequence<T>& Y, T c, T s) {
            size_t incx = X.increment(), incy = Y.increment(), n = Y.length();
            apply(incx, X.begin(), incx, Y.begin(), incy, c, s);
        }

    private:

        void apply(size_t n, T* X, size_t incx, T* Y, size_t incy, T c, T s);
    };

    template <typename T>
    void ROT<T>::apply(size_t n, T* X, size_t incx, T* Y, size_t incy, T c, T s) {
        if (n == 0)
            return;
        T* y = Y;
        T* x = X;
        T* const e = X + incx * n;
        while (x != e) {
            double xcur = *x, ycur = *y;
            (*x) = c * xcur + s * ycur;
            (*y) = c * ycur - s * xcur;
            y += incy;
            x += incx;
        }
    }

}
#endif
