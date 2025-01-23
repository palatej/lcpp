#ifndef __lcpp_gemv_h
#define __lcpp_gemv_h

#include "matrix.h"
#include "dot.h"
#include "scal.h"

namespace LCPP {
    /// <summary>
    /// Compute y : = alpha * A * x + beta * y, or y : = alpha * A' * x + beta * y,
    /// where alpha and beta are scalars, x and y are vectors and A is an m by n matrix.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class GEMV {
    public:

        GEMV() {}
        void operator() (bool tA, T alpha, NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<T> x, T beta, NUMCPP::Sequence<T> y);

    };

    template <typename T>
    void GEMV<T>::operator() (bool tA, T alpha, NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<T> x, T beta, NUMCPP::Sequence<T> y) {
        int m = A.getNrows(), n = A.getNcols();
        int leny;
        if (tA) {
            leny = n;
        }
        else {
            leny = m;
        }
        T zero = NUMCPP::CONSTANTS<T>::zero, one = NUMCPP::CONSTANTS<T>::one;
        int incy=y.increment(), incx=x.increment();
        T* X = x.begin(), * Y = y.begin();
        if (beta == zero) {
            NUMCPP::Sequence<T>::set(leny, zero, Y, incy);
        } else if(beta != one) {
            NUMCPP::Sequence<T>::mul(leny, beta, Y, incy);
        }
        if (alpha == zero)
            return;
        if (tA) {
            SequenceIterator<T> iter=A.rowsIterator();
            while (iter.hasNext()) {
                y.addAY(alpha * (*X), iter.next());
                X += incx;
            }
        }
        else {
            SequenceIterator<T> iter=A.columnsIterator();
            while (iter.hasNext()) {
                y.addAY(alpha * (*X), iter.next());
                X += incx;
            }
        }
        
    }

}

#endif