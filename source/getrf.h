#ifndef __lcpp_getrf_h
#define __lcpp_getrf_h

#include "getrf2.h"


namespace LCPP {
	template<typename T>
	class GETRF {
	public:

		GETRF() {}

		void operator()(NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<T> pivots);

	private:

		int blockSize(int m, int n) {
			return 1;
		}
		 
	};

	template<typename T>
	void GETRF<T>::operator()(NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<T> pivots) {
		if (A.isEmpty())
			return;
		int m = A.getNrows(), n = A.getNcols;
		int nb = blockSize(m, n);
		GETRF2<T> getrf2;
		if (nb == 1) {
			getrf2(A, pivots);
		}
		else {
			throw std::exception("Not implemented yet");
		}
	}
}

#endif 