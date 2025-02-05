#ifndef __lcpp_trsm_h
#define __lcpp_trsm_h

#include "matrix.h"
#include "matrix_0.h"

namespace LCPP {

    /// <summary>
    /// op( A )*X = alpha*B (left),   or   X*op( A ) = alpha*B (right)
    /// op( A ) = A   or   op( A ) = A'
    /// A is a unit, or non-unit, upper or lower triangular matrix
    /// X is overwritten on B
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class TRSM {
    public:

        TRSM() {}

        void operator()(Side side, Triangular uplo, bool tA, bool unitdiag, NUMCPP::FastMatrix<T> A, T alpha, NUMCPP::FastMatrix<T> B);

    private:

    };

    template<typename T>
    void TRSM<T>::operator() (Side side, Triangular uplo, bool tA, bool unitdiag, NUMCPP::FastMatrix<T> A, T alpha, NUMCPP::FastMatrix<T> B) {
        if (B.isEmpty())
            return;
        if (alpha == NUMCPP::CONSTANTS<T>::zero) {
            B.set(NUMCPP::CONSTANTS<T>::zero);
            return;
        }
        int m = B.getNrows(), n = B.getNcols();
        // 
        if (!A.isSquare())
            throw std::exception("Invalid matrix in trsm");
        if (side == Side::Right) {
            if (A.getNrows() != n)
                throw std::exception("Invalid matrix in trsm");
        }
        else {
            if (A.getNrows() != m)
                throw std::exception("Invalid matrix in trsm");
        }

        int lda = A.getColumnIncrement();
        const T* a = A.cptr();
        if (side == Side::Left) {
            if (!tA) {
                NUMCPP::SequenceIterator<T> bcols = B.columnsIterator();
                //  A * X = alpha * B
                if (uplo == Triangular::Upper) {
                    while (bcols.hasNext()) {
                        NUMCPP::Sequence<T> col = bcols.next();
                        col.mul(alpha);
                        T* b = col.start();
                        for (int k = m - 1; k >= 0; --k) {
                            const T* ca = a + k * lda;
                            T bk = b[k];
                            if (bk != NUMCPP::CONSTANTS<T>::zero) {
                                if (!unitdiag) {
                                    bk /= ca[k];
                                    b[k] = bk;
                                }
                                for (int i = 0; i < k; ++i) {
                                    b[i] -= ca[i] * bk;
                                }
                            }
                        }
                    }
                }
                else {
                    while (bcols.hasNext()) {
                        NUMCPP::Sequence<T> col = bcols.next();
                        col.mul(alpha);
                        T* b = col.start();
                        for (int k = 0; k < m; ++k) {
                            const T* ca = a + k * lda;
                            T bk = b[k];
                            if (bk != NUMCPP::CONSTANTS<T>::zero) {
                                if (!unitdiag) {
                                    bk /= ca[k];
                                    b[k] = bk;
                                }
                                for (int i = k + 1; i < m; ++i) {
                                    b[i] -= ca[i] * bk;
                                }
                            }
                        }
                    }
                }
            }
            else {
                //  A' * X = alpha * B
                if (uplo == Triangular::Upper) {
                    T* b = B.ptr();
                    for (int j = 0; j < n; ++j, b += B.getColumnIncrement()) {
                        const T* ca = a;
                        for (int i = 0; i < m; ++i, ca += lda) {
                            T tmp = alpha * b[i];
                            for (int k = 0; k < i; ++k) {
                                tmp -= ca[k] * b[k];
                            }
                            if (!unitdiag)
                                tmp /= ca[i];
                            b[i] = tmp;
                        }
                    }
                }
                else {
                    T* b = B.ptr();
                    for (int j = 0; j < n; ++j, b += B.getColumnIncrement()) {
                        const T* ca = a + (m - 1) * lda;
                        for (int i = m - 1; i >= 0; --i, ca -= lda) {
                            T tmp = alpha * b[i];
                            for (int k = i + 1; k < m; ++k) {
                                tmp -= ca[k] * b[k];
                            }
                            if (!unitdiag)
                                tmp /= ca[i];
                            b[i] = tmp;
                        }

                    }
                }
            }
        }
        else {
            if (!tA) {
                //  X * A = alpha * B or X = alpha * B * inv(A)
                if (uplo == Triangular::Upper) {
                    NUMCPP::SequenceIterator<T> bcols = B.columnsIterator(),
                        acols = A.columnsIterator();
                    for (int j = 0; j < n; ++j) {
                        NUMCPP::Sequence<T> b = bcols.next();
                        b.mul(alpha);
                        NUMCPP::Sequence<T> a = acols.next();
                        NUMCPP::SequenceIterator<T> bj = B.columnsIterator();
                        for (int k = 0; k < j; ++k) {
                            b.addAY(-a(k), bj.next());
                        }
                        if (!unitdiag)
                            b.div(a(j));
                    }
                }
                else {
                    NUMCPP::SequenceIterator<T> bcols = B.reverseColumnsIterator(),
                        acols = A.columnsIterator();
                    for (int j = n - 1; j >= 0; --j) {
                        NUMCPP::Sequence<T> b = bcols.next();
                        b.mul(alpha);
                        NUMCPP::Sequence<T> a = acols.next();
                        NUMCPP::SequenceIterator<T> bj = B.reverseColumnsIterator();
                        for (int k = n - 1; k > j; --k) {
                            b.addAY(-a(k), bj.next());
                        }
                        if (!unitdiag)
                            b.div(a(j));
                    }
                }
            }
            else {
                //  X * A' = alpha * B  or X = alpha * B * inv(A')
                if (uplo == Triangular::Upper) {
                    NUMCPP::SequenceIterator<T> bcols = B.reverseColumnsIterator(),
                        acols = A.reverseColumnsIterator();
                    for (int k = n - 1; k >= 0; --k) {
                        NUMCPP::Sequence<T> b = bcols.next();
                        NUMCPP::Sequence<T> a = acols.next();
                        if (!unitdiag)
                            b.div(a(k));
                        NUMCPP::SequenceIterator<T> bj = B.columnsIterator();
                        for (int j = 0; j < k; ++j) {
                            bj.next().addAY(-a(j), b);
                        }
                        b.mul(alpha);
                    }
                }
                else {
                    NUMCPP::SequenceIterator<T> bcols = B.columnsIterator(),
                        acols = A.columnsIterator();
                    for (int k = 0; k < n; ++k) {
                        NUMCPP::Sequence<T> b = bcols.next();
                        NUMCPP::Sequence<T> a = acols.next();
                        if (!unitdiag)
                            b.div(a(k));
                        NUMCPP::SequenceIterator<T> bj = B.reverseColumnsIterator();
                        for (int j = n - 1; j > k; --j) {
                            bj.next().addAY(-a(j), b);
                        }
                        b.mul(alpha);
                    }
                }

            }
        }

    }
}

#endif
