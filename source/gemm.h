#ifndef __lcpp_gemm_h
#define __lcpp_gemm_h

#include "matrix.h"

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

        void set(int m, int n, T value, T* C, int ldc);
        void mul(int m, int n, T beta, T* C, int ldc);

        void operator() (bool tA, bool tB, T alpha, const NUMCPP::FastMatrix<T>& A, const NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C);
        void operator() (bool tA, bool tB, T alpha, const NUMCPP::Matrix<T>& A, const NUMCPP::Matrix<T>& B, T beta, NUMCPP::Matrix<T>& C) {
            const NUMCPP::FastMatrix<double> a = A.all(), b = B.all();
            NUMCPP::FastMatrix<double> c = C.all();
            (*this)(tA, tB, alpha, a, b, beta, c);
        }

    private:

        void apply(bool tA, bool tB, int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc);
        void apply_ab(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc);
        void apply_tab(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc);
        void apply_atb(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc);
        void apply_tatb(int m, int n, int k, T alpha, const T* A, int lda, const T* B, int ldb, T beta, T* C, int ldc);

        static const T zero, one;

    };

    template <typename T>
    const T GEMM<T>::zero = NUMCPP::CONSTANTS<T>::zero;
    
    template <typename T>
    const T GEMM<T>::one = NUMCPP::CONSTANTS<T>::one;


    template<typename T>
    void GEMM<T>::operator() (bool tA, bool tB, T alpha, const NUMCPP::FastMatrix<T>& A, const NUMCPP::FastMatrix<T>& B, T beta, NUMCPP::FastMatrix<T>& C) {
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

        apply(tA, tB, m, n, ka, alpha, A.cptr(), A.getLda(), B.cptr(), B.getLda(), beta, C.ptr(), C.getLda());
    }


    template<typename T>
    void GEMM<T>::set(int m, int n, T value, T* C, int ldc) {
        T* cstart = C;
        T* const end = C + ldc * n;
        while (cstart != end) {
            T* cur = cstart;
            T* const cend = cur + m;
            while (cur != cend)
                *cur++ = value;
            cstart += ldc;
        }
    }

    template<typename T>
    void GEMM<T>::mul(int m, int n, T value, T* C, int ldc) {
        if (value == NUMCPP::CONSTANTS<T>::one)
            return;
        if (value == NUMCPP::CONSTANTS<T>::zero) {
            set(m, n, value, C, ldc);
            return;
        }
        T* cstart = C;
        T* const end = C + ldc * n;
        while (cstart != end) {
            T* cur = cstart;
            T* const cend = cur + m;
            while (cur != cend)
                *cur++ *= value;
            cstart += ldc;
        }
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
                acur ++;
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
                acur ++;
            }
            cstart += ldc;
            bstart ++;
        }
    }
}


#endif
