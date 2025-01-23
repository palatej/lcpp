#ifndef __lcpp_potrf_h
#define __lcpp_potrf_h

#include "matrix.h"
#include "matrix_0.h"
#include "scal.h"
#include "gemv.h"
#include "dot.h"

namespace LCPP {
	/// <summary>
	/// Computes the Cholesky factorization of a real symmetric positive definite matrix A.
	/// The factorization has the form
	/// A = U' * U or A = L * L', where U is an upper triangular matrix and L is lower triangular.
	/// This is the block version of the algorithm.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class POTRF {
	public:

		POTRF() {}

		void operator()(Triangular uplo, NUMCPP::FastMatrix<T>& A);
		void operator()(Triangular uplo, NUMCPP::Matrix<T>& A) {
			FastMatrix<T>a = A.all();
			(*this)(uplo, a);
		}

		int info() const {
			return m_info;
		}

		void lcholesky(NUMCPP::FastMatrix<T>& A);
		void ucholesky(NUMCPP::FastMatrix<T>& A);

	private:

		int m_info;

	};

	template<typename T>
	void POTRF<T>::operator()(Triangular uplo, NUMCPP::FastMatrix<T>& A) {
		if (!A.isSquare())
			throw std::exception("Not square matrix in Cholesky");
		if (A.isEmpty())
			return;
		if (uplo == Triangular::Lower)
			lcholesky(A);
		else
			ucholesky(A);
	}

	template<typename T>
	void POTRF<T>::lcholesky(NUMCPP::FastMatrix<T>& A) {
		m_info = 0;

	}

	template<typename T>
	void POTRF<T>::ucholesky(NUMCPP::FastMatrix<T>& A) {
		m_info = 0;
	}

}

#endif
