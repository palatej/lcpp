#ifndef __lcpp_potrf2_h
#define __lcpp_potrf2_h

#include "matrix.h"
#include "matrix_0.h"

namespace LCPP {
	/// <summary>
	/// Computes the Cholesky factorization of a real symmetric positive definite matrix A.
	/// The factorization has the form
	/// A = U' * U or A = L * L', where U is an upper triangular matrix and L is lower triangular.
	/// This is the recursive version of the algorithm.It divides the matrix into four submatrices :
	///     [A11|A12]  where A11 is n1 by n1 and A22 is n2 by n2
	/// A = [---|---]  with n1 = n / 2
	///     [A21|A22]       n2 = n - n1
	/// 
	/// The subroutine calls itself to factor A11, update and scale A21 or A12, 
	/// update A22 then calls itself to factor A22.
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class POTRF2 {
	public:

		POTRF2() {}

		void operator()(Triangular uplo, NUMCPP::FastMatrix<T>& A);
		void operator()(Triangular uplo, NUMCPP::Matrix<T>& A) {
			FastMatrix<T>a = A.all();
			(*this)(uplo, a);
		}


		int info() const {
			return m_info;
		}

	private:

		int m_info;

	};

	template<typename T>
	void POTRF2<T>::operator()(Triangular uplo, NUMCPP::FastMatrix<T>& A) {

	}


}

#endif
