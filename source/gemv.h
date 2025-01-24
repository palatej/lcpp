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
        T zero = NUMCPP::CONSTANTS<T>::zero, one = NUMCPP::CONSTANTS<T>::one;
        if (beta == zero) {
            y.set(zero);
        } else if(beta != one) {
            y.mul(beta);
        }
        if (alpha == zero)
            return;
        if (tA) {
            NUMCPP::SequenceIterator<T> iter=A.rowsIterator();
            auto biter = x.cbegin();
            while (iter.hasNext()) {
                y.addAY(alpha * (*biter++), iter.next());
            }
        }
        else {
            NUMCPP::SequenceIterator<T> iter=A.columnsIterator();
            auto biter = x.cbegin();
            while (iter.hasNext()) {
                y.addAY(alpha * (*biter++), iter.next());
            }
        }
        
    }

}

#endif