#ifndef __lcpp_copy_h
#define __lcpp_copy_h

#include "sequence.h"

namespace LCPP {

    template <typename T>
    class COPY {
    public:

        COPY() {}

        void operator()(int n, const T* X, int incx, T* Y, int incy) {
            apply(n, X, incx, Y, incy);
        }

        void operator()(int n, const T* X, T* Y) {
            apply(n, X, Y);
        }

    private:

        void apply(int n, const T* X, int incx, T* Y, int incy);

        void apply(int n, const T* X, T* Y);

    };


    template <typename T>
    void COPY<T>::apply(int n, const T* X, int incx, T* Y, int incy) {
        int imax = incx * n;
        for (int i = 0, j = 0; i != imax; i += incx, j += incy) {
            Y[j] = X[i];
        }
    }

    template <typename T>
    void COPY<T>::apply(int n, const T* X, T* Y) {
         for (int i = 0; i != n; ++i)
            Y[i] = X[i];
    }

}
#endif
