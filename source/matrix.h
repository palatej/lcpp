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

		FastMatrix(T* data, size_t nrows, size_t ncols, size_t lda) {
			this->m_data = data;
			this->m_lda = lda;
			this->m_nrows = nrows;
			this->m_ncols = ncols;
		}

		FastMatrix(T* data, size_t nrows, size_t ncols) {
			this->m_data = data;
			this->m_lda = nrows;
			this->m_nrows = nrows;
			this->m_ncols = ncols;
		}

		T operator()(size_t r, size_t c)const {
			return m_data[r + m_lda * c];
		}

		T& operator()(size_t r, size_t c) {
			return m_data[r + m_lda * c];
		}

		~FastMatrix() {}

		size_t getNrows()const {
			return m_nrows;
		}

		size_t getNcols()const {
			return m_ncols;
		}

		size_t getLda()const {
			return m_lda;
		}

		const T* cptr()const {
			return m_data;
		}

		T* ptr(){
			return m_data;
		}

		Sequence<T> row(size_t row) {
			return Sequence<T>(m_data + row, m_data + row + m_lda * m_ncols, m_lda);
		}

		Sequence<T> column(size_t col) {
			size_t start = col * m_lda;
			return Sequence<T>(m_data + start, m_data + start + m_nrows);
		}

		const Sequence<T> row(size_t row)const {
			return Sequence<T>(m_data + row, m_data + row + m_lda * m_ncols, m_lda);
		}

		const Sequence<T> column(size_t col)const {
			size_t start = col * m_lda;
			return Sequence<T>(m_data + start, m_data + start + m_nrows);
		}

		bool isEmpty()const {
			return m_ncols == 0 || m_nrows == 0;
		}

		bool isSquare()const {
			return m_ncols == m_nrows;
		}

		template<typename S>
		friend std::ostream& operator<< (std::ostream& stream, const FastMatrix<S>& matrix);

	private:

		T* m_data;
		size_t m_nrows, m_ncols, m_lda;

	};

	template<typename T>
	class Matrix
	{
	public:

		Matrix();
		Matrix(size_t nrows, size_t ncols);
		Matrix(size_t nrows, size_t ncols, std::function<T(size_t, size_t)> fn);
		Matrix(const Matrix<T>& matrix);

		Matrix<T>& operator=(const Matrix<T>& matrix);


		virtual ~Matrix();

		FastMatrix<T> all();
		const FastMatrix<T> all()const;

		size_t getNrows()const {
			return m_nrows;
		}

		size_t getNcols()const {
			return m_ncols;
		}

		T operator()(size_t r, size_t c)const {
			return m_data[r + m_nrows * c];
		}

		T& operator()(size_t r, size_t c) {
			return m_data[r + m_nrows * c];
		}

		void rand();

		Matrix<T>& operator=(T x);

		size_t size()const;

		Sequence<T> row(size_t row) {
			return Sequence<T>(m_data + row, m_data + row + m_nrows * m_ncols, m_nrows);
		}

		Sequence<T> column(size_t col) {
			size_t start = col * m_nrows;
			return Sequence<T>(m_data + start, m_data + start + m_nrows);
		}

		const Sequence<T> row(size_t row)const {
			return Sequence<T>(m_data + row, m_data + row + m_nrows * m_ncols, m_nrows);
		}

		const Sequence<T> column(size_t col)const {
			size_t start = col * m_nrows;
			return Sequence<T>(m_data + start, m_data + start + m_nrows);
		}

		template<typename S>
		friend Matrix<S> transpose(const FastMatrix<S>& M);

		template<typename S>
		friend std::ostream& operator<< (std::ostream& stream, const Matrix<S>& matrix);

	private:

		T* m_data;
		size_t m_nrows, m_ncols;

	};

	template<typename T>
	inline Matrix<T>::Matrix() {
		m_data = NULL;
		m_nrows = 0;
		m_ncols = 0;
	}

	template<typename T>
	inline FastMatrix<T> Matrix<T>::all() {
		return FastMatrix<T>(m_data, m_nrows, m_ncols);
	}

	template<typename T>
	inline const FastMatrix<T> Matrix<T>::all()const {
		return FastMatrix<T>(m_data, m_nrows, m_ncols);
	}

	template<typename T>
	inline size_t Matrix<T>::size()const {
		return m_nrows * m_ncols;
	}

	template<typename T>
	Matrix<T>::Matrix(size_t nrows, size_t ncols)
	{
		m_data = new T[nrows * ncols];
		this->m_nrows = nrows;
		this->m_ncols = ncols;
	}

	template<typename T>
	inline Matrix<T>::Matrix(size_t nrows, size_t ncols, std::function<T(size_t, size_t)> fn)
	{
		m_data = new T[nrows * ncols];
		this->m_nrows = nrows;
		this->m_ncols = ncols;
		for (size_t c = 0, j = 0; c < ncols; ++c) {
			for (size_t r = 0; r < nrows; ++r, ++j)
				m_data[j] = fn(r, c);
		}
	}

	template<typename T>
	Matrix<T>::Matrix(const Matrix<T>& matrix)
	{
		size_t size = matrix.m_nrows * matrix.m_ncols;
		m_data = new T[size];
		std::memcpy(m_data, matrix.m_data, size * sizeof(T));
		m_nrows = matrix.m_nrows;
		m_ncols = matrix.m_ncols;

	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matrix)
	{
		if (this != &matrix) {
			size_t size = matrix.m_nrows * matrix.m_ncols;
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
		size_t sz = size();
		for (size_t u = 0; u < sz; ++u)
			m_data[u] = dist(mt);
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator=(T x)
	{
		size_t sz = size();
		for (size_t u = 0; u < sz; ++u)
			m_data[u] = x;
		return *this;
	}

	template<typename T>
	Matrix<T> transpose(const FastMatrix<T>& M)
	{
		size_t nr = M.getNrows(), nc = M.getNcols();
		Matrix<T> R(nc, nr);
		for (size_t c = 0; c < nr; ++c) {
			const NUMCPP::Sequence<T> col = M.row(c);
			R.column(c).copy(col);
		}
		return R;
	}


	template<typename T>
	std::ostream& operator<< (std::ostream& stream, const Matrix<T>& matrix) {
		return stream << matrix.all();
	}

	template<typename T>
	std::ostream& operator<< (std::ostream& stream, const FastMatrix<T>& matrix) {
		if (!matrix.isEmpty())
			for (size_t i = 0; i < matrix.m_nrows; ++i) {
				stream << matrix(i, 0);
				for (size_t j = 1; j < matrix.m_ncols; ++j)
					stream << '\t' << matrix(i, j);
				stream << "\n\r";
			}
		return stream;
	}


}

#endif
