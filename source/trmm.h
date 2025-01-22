#ifndef __lcpp_trmm_h
#define __lcpp_trmm_h

#include "matrix.h"
#include "matrix_0.h"

namespace LCPP {
    /// <summary>
    /// performs one of the matrix-matrix operations
    /// B : = alpha * op(A) * B, or B : = alpha * B * op(A),
    /// where  alpha  is a scalar, B  is an m by n matrix, A  is a unit, or
    /// non - unit, upper or lower triangular matrix and op(A)  is one  of
    /// op(A) = A or op(A) = A'.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class TRMM {
    public:

        TRMM() {}
        
        void operator()(Side side, Triangular uplo, bool transa, bool diag, T alpha, NUMCPP::FastMatrix<T> A, NUMCPP::FastMatrix<T> B);
    };

    template <typename T>
    void TRMM<T>::operator()(Side side, Triangular uplo, bool transa, bool diag, T alpha, NUMCPP::FastMatrix<T> A, NUMCPP::FastMatrix<T> B){
    }
}

#endif
