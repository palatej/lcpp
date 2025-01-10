#ifndef __lcpp_asum_h
#define __lcpp_asum_h

#include "sequence.h"

namespace LCPP {

    template <typename T>
    class ASUM {
    public:

        ASUM() {}

        T operator()(size_t n, const T* X, size_t incx) {
            return apply(n, X, incx);
        }

        T operator()(size_t n, const T* X) {
            return apply(n, X, 1);
        }

        T operator()(const NUMCPP::Sequence<T>& X) {
            size_t incx = X.increment(), n = X.length();
            return apply(n, X.begin(), incx);
        }

    private:

        T apply(size_t n, const T* X, size_t incx);
    };


    template <typename T>
    T ASUM<T>::apply(size_t n, const T* X, size_t incx) {
        // Quick return if possible
        if (n == 0)
            return 0;
        T asum = 0;
        T* x = X;
        T* const e = X + incx * n;
        while (x != e) {
            asum += abs(*x);
            x += incx;
        }
        return asum;
    }

}
#endif
