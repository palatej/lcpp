#ifndef __lcpp_copy_h
#define __lcpp_copy_h

#include "sequence.h"

namespace LCPP {

    template <typename T>
    class COPY {
    public:

        COPY() {}

        void operator()(size_t n, const T* X, size_t incx, T* Y, size_t incy) {
            apply(n, X, incx, Y, incy);
        }

        void operator()(size_t n, const T* X, T* Y) {
            apply(n, X, Y);
        }

        void operator()(const NUMCPP::Sequence<T>& X, NUMCPP::Sequence<T>& Y) {
            size_t incx = X.increment(), incy = Y.increment(), n = Y.length();
            if (incx == 1 && incy == 1)
                apply(n, X.begin(), Y.begin());
            else
                apply(n, X.begin(), incx, Y.begin(), incy);
        }

    private:

        void apply(size_t n, const T* X, size_t incx, T* Y, size_t incy);

        void apply(size_t n, const T* X, T* Y);

    };


    template <typename T>
    void COPY<T>::apply(size_t n, const T* X, size_t incx, T* Y, size_t incy) {
        if (n == 0)
            return;
        T* y = Y;
        const T* x = X;
        const T* const e = X + incx * n;
        while (x != e) {
            *y = *x;
            y += incy;
            x += incx;
        }
    }

    template <typename T>
    void COPY<T>::apply(size_t n, const T* X, T* Y) {
        if (n == 0)
            return;
        T* y = Y;
        const T* x = X;
        const T* const e = X + n;
        while (x != e) {
            *y++ = *x++;
        }
    }

}
#endif
