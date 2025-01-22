#ifndef __lcpp_trsm_h
#define __lcpp_trsm_h

#include "matrix.h"
#include "matrix_0.h"

namespace LCPP {

    /// <summary>
    /// op( A )*X = alpha*B,   or   X*op( A ) = alpha*B
    /// op( A ) = A   or   op( A ) = A'
    /// A is a unit, or non-unit, upper or lower triangular matrix
    /// X is overwritten on B
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class TRSM {
    public:

        TRSM() {}

        void operator()(Side side, Triangular uplo, bool transa, bool diag, NUMCPP::FastMatrix<T> A, NUMCPP::FastMatrix<T> B);

    private:

    };

    template<typename T>
    void TRSM<T>::operator() (Side side, Triangular uplo, bool transa, bool diag, NUMCPP::FastMatrix<T> A, NUMCPP::FastMatrix<T> B) {

    }
}

#endif
