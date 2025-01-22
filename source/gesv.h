#ifndef __lcpp_gesv_h
#define __lcpp_gesv_h

#include "matrix.h"

namespace LCPP {
    /// <summary>
    /// GESV computes the solution to a real system of linear equations
    ///  A* X = B, where A is an N x N matrix and X and B are N x NRHS matrices.
    /// The LU decomposition with partial pivoting and row interchanges is
    /// used to factor A as
    /// A = P * L * U,
    /// where P is a permutation matrix, L is unit lower triangular, and U is
    /// upper triangular.The factored form of A is then used to solve the
    /// system of equations A* X = B.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class GESV {
    public:

        GESV() {}

        void operator()(bool tA, NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<int> piv, NUMCPP::FastMatrix<T> B);

    };


    template <typename T>
    void GESV<T>::operator()(bool tA, NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<int> piv, NUMCPP::FastMatrix<T> B) {

    }

}

#endif

