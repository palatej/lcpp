#ifndef __lcpp_getrf2_h
#define __lcpp_getrf2_h

#include "matrix.h"

namespace LCPP {

    /// <summary>
    /// DGETRF2 computes an LU factorization of a general MxN matrix A
    /// using partial pivoting with row interchanges.
    /// The factorization has the form A = P * L* U
    /// where P is a permutation matrix, L is lower triangular with unit
    /// diagonal elements(lower trapezoidal if m > n), and U is upper
    /// triangular(upper trapezoidal if m < n).
    ///  
    /// This is the recursive version of the algorithm.It divides
    /// the matrix into four submatrices :
    ///     [A11|A12] where A11 is n1 by n1 and A22 is n2 by n2
    /// A = [---|---]  with n1 = min(m, n) / 2
    ///     [A21|A22]       n2 = n - n1
    /// 
    ///                                      [A11]
    /// The subroutine calls itself to factor[---],
    ///                                      [A21]
    ///                 [A12]
    /// do the swaps on [---], solve A12, update A22,
    ///                 [A22]
    /// then calls itself to factor A22 and do the swaps on A21.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class GETRF2 {
    public:

        GETRF2() { }

        void operator()(NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<T> pivots);

        int info() {
            return m_info;
        }

    private:

        int m_info;

    };

    template<typename T>
    void GETRF2<T>::operator()(NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<T> pivots) {
        m_info = 0;
        if (A.isEmpty())
            return 0;
        int m = A.getNrows(), n = A.getNcols();
        if (m == 1) {
            pivots(0) = 0;
            if (A(0, 0) == 0)
                m_info=1;
        }
        else if (n == 1) {

        }
        else {
            // recursive code
            int n1 = min(m, n) / 2;
            int n2 = n - n1;
            GETRF2<T> rgtrf2;
            NUMCPP::FastMatrix A1=A.left(n1);
            rgtrf2(A1, pivots.left(n1));
        }
    }


}

#endif
