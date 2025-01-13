#ifndef __lcpp_larfg_h
#define __lcpp_larfg_h

#include "sequence.h"

namespace LCPP {
    /// <summary>
    /// Generates an elementary reflector(Householder matrix).
    ///       H * ( alpha ) = ( beta ),   H**T * H = I.
    ///           (   x   )   (   0  )
    /// </summary>
    class LARFG {
    public:
        template <typename T>
        static T apply(int n, T alpha, T* X, int incx);

        template <typename T>
        static T apply(int n, T alpha, T* X);

        template <typename T>
        static T apply( NUMCPP::Sequence<T>& X) {
            int incx = X.increment(), n = X.length();
            T* x = X.begin();
            if (incx == 1)
                return apply(n, *x, x+1);
            else
                return apply(n, *x, x + incx, incx);
        }

    private:

        LARFG();
    };


    template <typename T>
    T LARFG::apply(int n, T alpha, T* X, int incx) {

        return 0;
    }

    template <typename T>
    T LARFG::apply(int n, T alpha, T* X) {

        return 0;
    }

}
#endif


