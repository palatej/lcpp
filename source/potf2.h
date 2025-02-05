#ifndef __lcpp_potf2_h
#define __lcpp_potf2_h

#include "matrix.h"
#include "matrix_0.h"

#include "dot.h"

namespace LCPP {
	/// <summary>
	/// Computes the Cholesky factorization of a real symmetric positive definite matrix A.
	/// The factorization has the form
	/// A = U' * U or A = L * L', where U is an upper triangular matrix and L is lower triangular.
	/// This is the unblocked version of the algorithm.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class POTF2 {
	public:

		POTF2() {}

		void operator()(Triangular uplo, NUMCPP::FastMatrix<T> A) {
			if (!A.isSquare())
				throw std::exception("Not square");
			if (Triangular::Lower == uplo)
				lcholesky(A);
			else
				ucholesky(A);
		}
		void operator()(Triangular uplo, NUMCPP::Matrix<T>& A) {
			(*this)(uplo, A.all());
		}
		

		int info() const{
			return m_info;
		}

		void lcholesky(NUMCPP::FastMatrix<T> A);
		void ucholesky(NUMCPP::FastMatrix<T> A);

	private:


		int m_info;

	};

	template<typename T>
	void POTF2<T>::lcholesky(NUMCPP::FastMatrix<T> A) {
		m_info = 0;
		if (A.isEmpty())
			return;
		int n = A.getNrows();
		int lda = A.getColumnIncrement();
		T zero = NUMCPP::CONSTANTS<T>::zero;
		T* a = A.ptr();
		if (n == 1) {
			if (*a <= zero) {
				m_info = -1;
			}
			else
				*a = std::sqrt(*a);
			return;
		}
		DOT<T> dot;
		auto diag=A.diagonal.cbegin();
		for (int j = 0; j < n; ++j) {
			T* aj = a + j;
			// L(j,j) = A(j,j)-dot(A(j,[0, j-1[)
			T ajj = *diag-dot(j, aj, lda, aj, lda);
			if (ajj <= zero || ajj != ajj) {
				*diag = ajj;
				m_info = j - n;
				return; 
			}
			ajj=std::sqrt(ajj);
			*diag++ = ajj;
		}
	
	}

	template<typename T>
	void POTF2<T>::ucholesky(NUMCPP::FastMatrix<T> A) {
		if (A.isEmpty())
			return;
	}
}

#endif
