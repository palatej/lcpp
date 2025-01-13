#ifndef __lcpp_nrm2_h
#define __lcpp_nrm2_h

#include "sequence.h"

namespace LCPP {

    /// <summary>
    /// Computes sqrt(x'x)
    /// </summary>
    template <typename T, typename S>
    class NRM2 {
    public:

        NRM2() {}

        S operator()(int n, const T* X, int incx) {
            return apply(n, X, incx);
        }

        S operator()(int n, const T* X) {
            return apply(n, X, 1);
        }

        S operator()(const NUMCPP::Sequence<T>& X) {
            int incx = X.increment(), n = X.length();
            return apply(n, X.begin(), incx);
        }

    private:

        S apply(int n, const T* X, int incx);
    };


    template <typename T, typename S>
    S NRM2<T, S>::apply(int n, const T* X, int incx) {
        // Quick return if possible
        S zero = NUMCPP::CONSTANTS<S>::zero;
        if (n == 0)
            return zero;
        bool notbig = true;
        S one = NUMCPP::CONSTANTS<S>::one;
        S maxn = NUMCPP::CONSTANTS<S>::huge;
        S tbig=NUMCPP::CONSTANTS<S>::tbig;
        S tsml = NUMCPP::CONSTANTS<S>::tsml;
        S sbig = NUMCPP::CONSTANTS<S>::sbig;
        S ssml = NUMCPP::CONSTANTS<S>::ssml;
        S asml = zero;
        S amed = zero;
        S abig = zero;
        S scl = one;
        S sumsq = zero;
        S ymin, ymax;
        const T* x = X;
        const T* const e = X + incx * n;
        while (x != e) {
            double ax = abs(*x);
            if (ax > tbig) {
                double tmp = ax * sbig;
                abig += tmp * tmp;
                notbig = false;
            }
            else if (ax < tsml) {
                if (notbig) {
                    double tmp = ax * ssml;
                    asml += tmp * tmp;
                }
            }
            else {
                amed += ax * ax;
            }
            x += incx;
        }
        if (abig > zero) {
            if (amed > zero || amed > maxn || amed != amed) {
                abig += (amed * sbig) * sbig;
            }
            scl = one / sbig;
            sumsq = abig;
        }
        else if (asml > zero) {
            if (amed > zero || amed > maxn || amed != amed) {
                amed = sqrt(amed);
                asml = sqrt(asml) / ssml;
                if (asml > amed) {
                    ymin = amed;
                    ymax = asml;
                }
                else {
                    ymin = asml;
                    ymax = amed;
                }
                scl = one;
                double tmp = ymin / ymax;
                sumsq = ymax * ymax * (one + tmp * tmp);
            }
            else {
                scl = one / ssml;
                sumsq = asml;
            }
        }
        else {
            scl = one;
            sumsq = amed;
        }
        return scl * sqrt(sumsq);
    }


}
#endif
