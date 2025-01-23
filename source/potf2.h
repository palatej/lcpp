#ifndef __lcpp_potf2_h
#define __lcpp_potf2_h

#include "matrix.h"
#include "matrix_0.h"

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

		void operator()(Triangular uplo, NUMCPP::FastMatrix<T>& A);
		void operator()(Triangular uplo, NUMCPP::Matrix<T>& A) {
			FastMatrix<T>a = A.all();
			(*this)(uplo, a);
		}
		

		int info() const{
			return m_info;
		}

	private:

		int m_info;

	};

	template<typename T>
	void POTF2<T>::operator()(Triangular uplo, NUMCPP::FastMatrix<T>& A) {

	}

}

#endif
