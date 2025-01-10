#ifndef __lcpp_scal_h
#define __lcpp_scal_h

#include "sequence.h"

namespace LCPP {

    template <typename T>
    class SCAL {
    public:

        SCAL() {}

        void operator()(size_t n, T a, T* X, size_t incx) {
            apply(n, a, X, incx);
        }

        void operator()(size_t n, T a, T* X) {
            apply(n, a, X, 1);
        }

        void operator()(T a, NUMCPP::Sequence<T>& X) {
            size_t incx = X.increment();
            apply(X.length(), a, X.begin(), incx);
        }

    private:

        void apply(size_t n, T a, T* X, size_t incx);
    };

    template <typename T>
    void SCAL<T>::apply(size_t n, T a, T* X, size_t incx) {
        // Quick return if possible
        if (n == 0 || a == 1)
            return;
        T* x = X;
        const T* const e = x + incx * n;
        while (x != e) {
            (*x) *= a;
            x += incx;
        }
    }
}

#endif


