#ifndef __lcpp_asum_h
#define __lcpp_asum_h

#include "sequence.h"

namespace LCPP {

    template <typename T>
    class ASUM {
    public:

        ASUM() {}

        T operator()(int n, const T* X, int incx) {
            return apply(n, X, incx);
        }

        T operator()(int n, const T* X) {
            return apply(n, X, 1);
        }

        T operator()(const NUMCPP::Sequence<T>& X) {
            int incx = X.increment(), n = X.length();
            return apply(n, X.begin(), incx);
        }

    private:

        T apply(int n, const T* X, int incx);
    };


    template <typename T>
    T ASUM<T>::apply(int n, const T* X, int incx) {
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
