#ifndef __lcpp_swap_h
#define __lcpp_swap_h

#include "sequence.h"

namespace LCPP {

        template <typename T>
        class SWAP {
        public:

            SWAP() {}

            void operator()(int n, T* X, int incx, T* Y, int incy) {
                apply(n, X, incx, Y, incy);
            }

            void operator()(int n, T* X, T* Y) {
                apply(n, X, 1, Y, 1);
            }

            void operator()(NUMCPP::Sequence<T>& X, NUMCPP::Sequence<T>& Y) {
                int incx = X.increment(), incy = Y.increment(), n = Y.length();
                apply(n, X.begin(), incx, Y.begin(), incy);
            }

        private:

            void apply(int n, T* X, int incx, T* Y, int incy);


        };


    template <typename T>
    void SWAP<T>::apply(int n, T* X, int incx, T* Y, int incy) {
        if (n == 0)
            return;
        T* y = Y;
        T* x = X;
        T* const e = X + incx * n;
        while (x != e) {
            T tmp = *x;
            *x = *y;
            *y = tmp;
            y += incy;
            x += incx;
        }
    }
}
#endif
