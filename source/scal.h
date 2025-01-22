#ifndef __lcpp_scal_h
#define __lcpp_scal_h

#include "sequence.h"

namespace LCPP {

    template <typename T>
    class SCAL {
    public:

        SCAL() {}

        void operator()(int n, T a, T* X, int incx) {
            apply(n, a, X, incx);
        }

        void operator()(int n, T a, T* X) {
            apply(n, a, X, 1);
        }

        void operator()(T a, NUMCPP::Sequence<T> X) {
            int incx = X.increment();
            apply(X.length(), a, X.begin(), incx);
        }

    private:

        void apply(int n, T a, T* X, int incx);
    };

    template <typename T>
    void SCAL<T>::apply(int n, T a, T* X, int incx) {
        // Quick return if possible
        if (n == 0 || a == 1)
            return;
        T* x = X;
        const T* const e = x + incx * n;
        if (a == 0) {
            while (x != e) {
                (*x) = 0;
                x += incx;
            }
        }
        else {
            while (x != e) {
                (*x) *= a;
                x += incx;
            }
        }
    }
}

#endif


