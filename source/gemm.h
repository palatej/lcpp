#ifndef __lcpp_gemm_h
#define __lcpp_gemm_h

#include "matrix.h"
#include "dot.h"
#include "scal.h"

namespace LCPP {
    /// <summary>
    /// Compute C:= alpha * op(A) * op(B) + beta * C, with op(X) = X or op(X) = X'
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class GEMM {
    public:

        GEMM() {}

        void operator() (bool tA, bool tB, int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc) {
            apply(tA, tB, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
        }

        void operator() (bool tA, bool tB, T alpha, NUMCPP::FastMatrix<T> A, NUMCPP::FastMatrix<T> B, T beta, NUMCPP::FastMatrix<T> C);
        void operator() (bool tA, bool tB, T alpha, NUMCPP::Matrix<T>& A, NUMCPP::Matrix<T>& B, T beta, NUMCPP::Matrix<T>& C) {
            (*this)(tA, tB, alpha, A.all(), B.all(), beta, C.all());
        }

    private:

        void apply(bool tA, bool tB, int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc);
        void apply_ab(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc);
        void apply_tab(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc);
        void apply_atb(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc);
        void apply_tatb(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc);

        static const T zero, one;

        void apply(bool tA, bool tB, T alpha, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C);
        void apply_ab(T alpha, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C);
        void apply_atb(T alpha, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C);
        void apply_tab(T alpha, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C);
        void apply_tatb(T alpha, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C);

    };

    template <typename T>
    const T GEMM<T>::zero = NUMCPP::CONSTANTS<T>::zero;

    template <typename T>
    const T GEMM<T>::one = NUMCPP::CONSTANTS<T>::one;


    template<typename T>
    void GEMM<T>::operator() (bool tA, bool tB, T alpha, NUMCPP::FastMatrix<T> A, NUMCPP::FastMatrix<T> B, T beta, NUMCPP::FastMatrix<T> C) {
        int m = C.getNrows(), n = C.getNcols();
        int ma, ka, kb, nb;
        if (tA) {
            ka = A.getNrows();
            ma = A.getNcols();
        }
        else {
            ma = A.getNrows();
            ka = A.getNcols();
        }
        if (tB) {
            nb = B.getNrows();
            kb = B.getNcols();
        }
        else {
            kb = B.getNrows();
            nb = B.getNcols();
        }
        if (ma != m || nb != n || ka != kb)
            throw std::invalid_argument("invalid dimensions in GEMM");

        apply(tA, tB, alpha, A, B, beta, C);
    }



    template<typename T>
    void GEMM<T>::apply(bool tA, bool tB, int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc) {
        // we skip the check of the dimensions in this low level routine. Should be done before
        if (m == 0 || n == 0)
            return;
        if (alpha == zero || k == 0) {
            mul(m, n, beta, C, ldc);
            return;
        }
        if (!tA) {
            if (!tB)
                apply_ab(m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
            else
                apply_atb(m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
        }
        else {
            if (!tB)
                apply_tab(m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
            else
                apply_tatb(m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
        }
    }

    template<typename T>
    void GEMM<T>::apply_ab(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc) {
        T* cstart = C;
        T* const cend = cstart + ldc * n;
        const T* bstart = B;
        const T* astart = A;
        while (cstart != cend) {
            // cur = C[.,j]
            T* cur = cstart;
            T* const end = cur + m;
            if (beta == zero)
                while (cur != end) { *cur++ = 0; }
            else if (beta != one)
                while (cur != end) { *cur++ *= beta; }
            cur = cstart;
            // bcur = bcur[., j]
            const T* bcur = bstart;
            const T* const bend = bcur + k;
            const T* acur = astart;
            while (bcur != bend) {
                T temp = alpha * (*bcur++);
                const T* a = acur;
                cur = cstart;
                while (cur != end) {
                    *cur++ += temp * *a++;
                }
                acur += lda;
            }
            cstart += ldc;
            bstart += ldb;
        }
    }

    template<typename T>
    void GEMM<T>::apply_atb(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc) {
        T* cstart = C;
        T* const cend = cstart + ldc * n;
        const T* bstart = B;
        const T* astart = A;
        while (cstart != cend) {
            // cur = C[.,j]
            T* cur = cstart;
            T* const end = cur + m;
            if (beta == zero)
                while (cur != end) { *cur++ = 0; }
            else if (beta != one)
                while (cur != end) { *cur++ *= beta; }
            cur = cstart;
            // bcur = bcur[., j]
            const T* bcur = bstart;
            const T* const bend = bcur + k * ldb;
            const T* acur = astart;
            while (bcur != bend) {
                T temp = alpha * (*bcur);
                const T* a = acur;
                cur = cstart;
                while (cur != end) {
                    *cur++ += temp * *a++;
                }
                acur += lda;
                bcur += ldb;
            }
            cstart += ldc;
            ++bstart;
        }
    }

    template<typename T>
    void GEMM<T>::apply_tab(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc) {
        T* cstart = C;
        T* const cend = cstart + ldc * n;
        const T* bstart = B;
        const T* astart = A;
        while (cstart != cend) {
            // cur = C[.,j]
            T* cur = cstart;
            T* const end = cur + m;
            if (beta == zero)
                while (cur != end) { *cur++ = 0; }
            else if (beta != one)
                while (cur != end) { *cur++ *= beta; }
            cur = cstart;
            // bcur = bcur[., j]
            const T* bcur = bstart;
            const T* const bend = bcur + k;
            const T* acur = astart;
            while (bcur != bend) {
                T temp = alpha * (*bcur++);
                const T* a = acur;
                cur = cstart;
                while (cur != end) {
                    *cur++ += temp * *a;
                    a += lda;
                }
                acur++;
            }
            cstart += ldc;
            bstart += ldb;
        }
    }

    template<typename T>
    void GEMM<T>::apply_tatb(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc) {
        T* cstart = C;
        T* const cend = cstart + ldc * n;
        const T* bstart = B;
        const T* astart = A;
        while (cstart != cend) {
            // cur = C[.,j]
            T* cur = cstart;
            T* const end = cur + m;
            if (beta == zero)
                while (cur != end) { *cur++ = 0; }
            else if (beta != one)
                while (cur != end) { *cur++ *= beta; }
            cur = cstart;
            const T* bcur = bstart;
            const T* const bend = bcur + k * ldb;
            const T* acur = astart;
            while (bcur != bend) {
                T temp = alpha * (*bcur);
                const T* a = acur;
                cur = cstart;
                while (cur != end) {
                    *cur++ += temp * *a;
                    a += lda;
                }
                bcur += ldb;
                acur++;
            }
            cstart += ldc;
            bstart++;
        }
    }

    template<typename T>
    void GEMM<T>::apply(bool tA, bool tB, T alpha, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C) {
        // we skip the check of the dimensions in this low level routine. Should be done before
        if (C.isEmpty())
            return;
        if (alpha == zero || A.isEmpty()) {
            NUMCPP::SequenceIterator<T> cols = C.columnsIterator();
            while (cols.hasNext())
                cols.next().mul(beta);
            return;
        }
        if (!tA) {
            if (!tB)
                apply_ab(alpha, A, B, beta, C);
            else
                apply_atb(alpha, A, B, beta, C);
        }
        else {
            if (!tB)
                apply_tab(alpha, A, B, beta, C);
            else
                apply_tatb(alpha, A, B, beta, C);
        }
    }

    template<typename T>
    void GEMM<T>::apply_ab(T alpha, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C) {
        NUMCPP::SequenceIterator<T> cols = C.columnsIterator();
        NUMCPP::SequenceIterator<T> bcols = B.columnsIterator();
        SCAL<T> scal;
        DOT<T, T> dot;
        while (cols.hasNext()) {
            NUMCPP::Sequence<T>& col = cols.next();
            col.mul(beta);
            NUMCPP::SequenceIterator<T> arows = A.rowsIterator();
            NUMCPP::Sequence<T>& bcol = bcols.next();
            auto cur = col.begin();
            while (arows.hasNext()) {
                *cur++ += alpha * dot(arows.next(), bcol);
            }
        }
    }

    template<typename T>
    void GEMM<T>::apply_atb(T alpha, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C) {
        NUMCPP::SequenceIterator<T> cols = C.columnsIterator();
        NUMCPP::SequenceIterator<T> bcols = B.rowsIterator();
        SCAL<T> scal;
        DOT<T, T> dot;
        while (cols.hasNext()) {
            NUMCPP::SequenceIterator<T> arows = A.rowsIterator();
            NUMCPP::Sequence<T>& col = cols.next();
            col.mul(beta);
            NUMCPP::Sequence<T>& bcol = bcols.next();
            auto cur = col.begin();
            while (arows.hasNext()) {
                *cur++ += alpha*dot(arows.next(), bcol);
            }
        }
    }

    template<typename T>
    void GEMM<T>::apply_tab(T alpha, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C) {
        NUMCPP::SequenceIterator<T> cols = C.columnsIterator();
        NUMCPP::SequenceIterator<T> bcols = B.columnsIterator();
        SCAL<T> scal;
        DOT<T, T> dot;
        while (cols.hasNext()) {
            NUMCPP::SequenceIterator<T> arows = A.columnsIterator();
            NUMCPP::Sequence<T>& col = cols.next();
            col.mul(beta);
            NUMCPP::Sequence<T>& bcol = bcols.next();
            auto cur = col.begin();
            while (arows.hasNext()) {
                *cur++ += alpha * dot(arows.next(), bcol);
            }
        }
    }
    template<typename T>
    void GEMM<T>::apply_tatb(T alpha, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C) {
        NUMCPP::SequenceIterator<T> cols = C.columnsIterator();
        NUMCPP::SequenceIterator<T> bcols = B.rowsIterator();
        SCAL<T> scal;
        DOT<T, T> dot;
        while (cols.hasNext()) {
            NUMCPP::SequenceIterator<T> arows = A.columnsIterator();
            NUMCPP::Sequence<T>& col = cols.next();
            col.mul(beta);
            NUMCPP::Sequence<T>& bcol = bcols.next();
            auto cur = col.begin();
            while (arows.hasNext()) {
                *cur++ += alpha * dot(arows.next(), bcol);
            }
        }
    }
}


#endif
