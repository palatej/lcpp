#ifndef __numcpp_matrix_h
#define __numcpp_matrix_h

#include <iostream>
#include <functional>
#include <random>
#include "sequence.h"

namespace NUMCPP {

	template <typename T>
	class FastMatrix
	{
	public:

		FastMatrix(T* data, int nrows, int ncols, int lda):
			m_data(data), m_nrows(nrows), m_ncols(ncols),m_lda(lda)
		{ }


		T& operator()(int r, int c) {
			return m_data[r + m_lda * c];
		}

		~FastMatrix() {}

		int getNrows() {
			return m_nrows;
		}

		int getNcols() {
			return m_ncols;
		}

		int getLda() {
			return m_lda;
		}

		T* ptr() {
			return m_data;
		}

		Sequence<T> row(int row) {
			return Sequence<T>(m_data + row, m_data + row + m_lda * m_ncols, m_lda);
		}

		Sequence<T> column(int col) {
			int start = col * m_lda;
			return Sequence<T>(m_data + start, m_data + start + m_nrows);
		}

		Sequence<T> diagonal() {
			int n = std::min(m_nrows, m_ncols), inc = 1 + m_lda;
			return Sequence<T>(m_data, m_data + inc * n, inc);
		}

		Sequence<T> subDiagonal(int pos);

		SequenceIterator<T> rowsIterator() {
			return SequenceIterator<T>(row(-1), m_nrows, 1);
		}

		SequenceIterator<T> columnsIterator() {
			return SequenceIterator<T>(column(-1), m_ncols, m_lda);
		}

		bool isEmpty() {
			return m_ncols == 0 || m_nrows == 0;
		}

		bool isSquare() {
			return m_ncols == m_nrows;
		}

		FastMatrix<T> left(int n) {
			return FastMatrix(m_data, m_lda, m_nrows, n);
		}

		FastMatrix<T> right(int n) {
			int nc = m_ncols - n;
			return FastMatrix(m_data+m_lda*nc, m_lda, m_nrows, n);
		}

		FastMatrix<T> top(int n) {
			return FastMatrix(m_data, m_lda, n, m_ncols);
		}

		FastMatrix<T> bottom(int n) {
			int nr = m_nrows - n;
			return FastMatrix(m_data+nr, m_lda, n, m_ncols);
		}

		void set(T value) {
			set(m_nrows, m_ncols, value, m_data, m_lda);
		}

		void set(std::function<T(int, int)> fn);

		void mul(T value) {
			mul(m_nrows, m_ncols, value, m_data, m_lda);
		}

		static void set(int m, int n, T value, T* C, int ldc);

		static void mul(int m, int n, T beta, T* C, int ldc);


		template<typename S>
		friend std::ostream& operator<< (std::ostream& stream, FastMatrix<S> matrix);

	private:

		T* m_data;
		int m_nrows, m_ncols, m_lda;

	};

	template<typename T>
	class Matrix
	{
	public:

		Matrix();
		Matrix(int nrows, int ncols);
		Matrix(int nrows, int ncols, std::function<T(int, int)> fn);
		Matrix(const Matrix<T>& matrix);

		Matrix<T>& operator=(const Matrix<T>& matrix);


		virtual ~Matrix();

		FastMatrix<T> all();

		int getNrows()const {
			return m_nrows;
		}

		int getNcols()const {
			return m_ncols;
		}

		T operator()(int r, int c)const {
			return m_data[r + m_nrows * c];
		}

		T& operator()(int r, int c) {
			return m_data[r + m_nrows * c];
		}

		void set(std::function<T(int, int)> fn) {
			FastMatrix<T> a = all();
			a.set(fn);
		}
		

		void rand();

		Matrix<T>& operator=(T x);

		int size()const;

		Sequence<T> row(int row) {
			return Sequence<T>(m_data + row, m_data + row + m_nrows * m_ncols, m_nrows);
		}

		Sequence<T> column(int col) {
			int start = col * m_nrows;
			return Sequence<T>(m_data + start, m_data + start + m_nrows);
		}

		template<typename S>
		friend Matrix<S> transpose(FastMatrix<S>& M);

		template<typename S>
		friend std::ostream& operator<< (std::ostream& stream, Matrix<S>& matrix);

	private:

		T* m_data;
		int m_nrows, m_ncols;

	};

	template<typename T>
	inline Matrix<T>::Matrix() {
		m_data = NULL;
		m_nrows = 0;
		m_ncols = 0;
	}

	template<typename T>
	inline FastMatrix<T> Matrix<T>::all() {
		return FastMatrix<T>(m_data, m_nrows, m_ncols, m_nrows);
	}

	template<typename T>
	Sequence<T> FastMatrix<T>::subDiagonal(int pos) {
		if (pos >= m_ncols) {
			return Sequence<T>(NULL, 0);
		}
		if (-pos >= m_nrows) {
			return Sequence<T>(NULL, 0);
		}
		int beg = 0, inc = 1 + m_lda;
		int n;
		if (pos > 0) {
			beg += pos * m_lda;
			n =std::min(m_nrows, m_ncols - pos);
		}
		else if (pos < 0) {
			beg -= pos;
			n = std::min(m_nrows + pos, m_ncols);
		}
		else {
			n = std::min(m_nrows, m_ncols);
		}
		return Sequence<T>(m_data+beg, m_data+beg + inc * n, inc);
	}

	template<typename T>
	inline int Matrix<T>::size()const {
		return m_nrows * m_ncols;
	}

	template<typename T>
	Matrix<T>::Matrix(int nrows, int ncols)
	{
		m_data = new T[nrows * ncols];
		this->m_nrows = nrows;
		this->m_ncols = ncols;
	}

	template<typename T>
	inline Matrix<T>::Matrix(int nrows, int ncols, std::function<T(int, int)> fn)
	{
		m_data = new T[nrows * ncols];
		m_nrows = nrows;
		m_ncols = ncols;
		for (int c = 0, j = 0; c < ncols; ++c) {
			for (int r = 0; r < nrows; ++r, ++j)
				m_data[j] = fn(r, c);
		}
	}

	template<typename T>
	Matrix<T>::Matrix(const Matrix<T>& matrix)
	{
		int size = matrix.m_nrows * matrix.m_ncols;
		m_data = new T[size];
		std::memcpy(m_data, matrix.m_data, size * sizeof(T));
		m_nrows = matrix.m_nrows;
		m_ncols = matrix.m_ncols;

	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matrix)
	{
		if (this != &matrix) {
			int size = matrix.m_nrows * matrix.m_ncols;
			delete[] m_data;
			m_data = new T[size];
			std::memcpy(m_data, matrix.m_data, size * sizeof(T));
			m_nrows = matrix.m_nrows;
			m_ncols = matrix.m_ncols;
		}

		return *this;
	}

	template<typename T>
	Matrix<T>::~Matrix() {
		delete[] m_data;
	}

	template<>
	inline void Matrix<double>::rand() {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(1.0, 10.0);
		int sz = size();
		for (int u = 0; u < sz; ++u)
			m_data[u] = dist(mt);
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator=(T x)
	{
		int sz = size();
		for (int u = 0; u < sz; ++u)
			m_data[u] = x;
		return *this;
	}

	template<typename T>
	Matrix<T> transpose(FastMatrix<T>& M)
	{
		int nr = M.getNrows(), nc = M.getNcols();
		Matrix<T> R(nc, nr);
		for (int c = 0; c < nr; ++c) {
			NUMCPP::Sequence<T> col = M.row(c);
			NUMCPP::Sequence<T> ncol=R.column(c);
			ncol.copy(col);
		}
		return R;
	}


	template<typename T>
	std::ostream& operator<< (std::ostream& stream, Matrix<T>& matrix) {
		return stream << matrix.all();
	}

	template<typename T>
	std::ostream& operator<< (std::ostream& stream, FastMatrix<T> matrix) {
		if (!matrix.isEmpty())
			for (int i = 0; i < matrix.m_nrows; ++i) {
				stream << matrix(i, 0);
				for (int j = 1; j < matrix.m_ncols; ++j)
					stream << '\t' << matrix(i, j);
				stream << "\n\r";
			}
		return stream;
	}

	template<typename T>
	void FastMatrix<T>::set(int m, int n, T value, T* C, int ldc) {
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
	void FastMatrix<T>::set(std::function<T(int, int)> fn)
	{
		T* data = m_data;
		for (int c = 0; c < m_ncols; ++c) {
			T* datac = data;
			for (int r = 0; r < m_nrows; ++r)
				*datac++ = fn(r, c);
			data += m_lda;
		}
	}

	template<typename T>
	void FastMatrix<T>::mul(int m, int n, T value, T* C, int ldc) {
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

}

#endif
