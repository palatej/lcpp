#ifndef __sequence_h
#define __sequence_h

#include <functional>
#include <algorithm>
#include <random>
#include <stdexcept>

#include "constants.h"

namespace NUMCPP {

    enum Fill {
        None, Sum, NegSum, Zero
    };

    template <typename T>
    class SequenceIterator;

    /// <summary>
    /// Sequence of elements of type T that cannot be modified
    /// To be noted that the sequence itself can be modified (not its items)
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class Sequence
    {
    public:


        Sequence() :m_data(nullptr), m_inc(0), m_n(0) {

        }

        Sequence(T* p0, T* p1, int inc) :m_data(p0), m_inc(inc) {
            m_n = (p1 - p0) / inc;
        }
        Sequence(T* p0, T* p1) :m_data(p0), m_inc(1), m_n(p1 - p0) {
        }

        Sequence(T* p0, int n, int inc) :m_data(p0), m_inc(inc), m_n(n) {
        }

        Sequence(T* p0, int n) :m_data(p0), m_inc(1), m_n(n) {
        }

        bool isEmpty() {
            return m_n == 0;
        }

        T* begin() {
            return m_data;
        }

        T* end() {
            return m_data + m_inc * m_n;
        }

        Sequence<T> left(int n) {
            return Sequence<T>(m_data, n, m_inc);
        }

        Sequence<T> right(int n) {

            return Sequence<T>(m_p0 + m_inc * (m_n - n), n, m_inc);
        }

        Sequence<T> drop(int nl, int nr) {
            int nc = nl + nr;
            if (nc >= m_n)
                return Sequence();
            return Sequence<T>(m_data + m_inc * nl, n - nc, m_inc);
        }

        Sequence<T> extend(int nl, int nr) {
            return drop(-nl, -nr);
        }

        Sequence<T> extract(int start, int n) {
            int nc = start + n;
            if (nc >= m_n)
                return Sequence();
            return Sequence<T>(m_data + m_inc * start, n, m_inc);
        }

        Sequence<T> reverse() {
            return Sequence<T>(m_data + (m_n - 1) * m_inc, m_n, -m_inc);
        }

        T& operator()(int idx) {
            return *(m_data + idx * m_inc);
        }

        void copy(Sequence<T> src);

        void rand();

        int length() {
            return m_n;
        }

        int increment() {
            return m_inc;
        }

        void copyTo(T* buffer);

        void slide(int del);

        void mul(T value) {
            mul(m_n, value, m_data, m_inc);
        }

        void add(T value) {
            add(m_n, value, m_data, m_inc);
        }

        void addAY(T a, Sequence<T> Y);

        void set(T value) {
            set(m_n, value, m_data, m_inc);
        }

        template<typename S>
        friend std::ostream& operator<< (std::ostream& stream, Sequence<S> seq);

        static void mul(int n, T value, T* x, int incx);

        static void add(int n, T value, T* x, int incx);

        static void set(int n, T value, T* x, int incx);

    private:

        T* m_data;
        int m_inc;
        int m_n;

    };


    template <typename T>
    inline void Sequence<T>::slide(int del) {
        m_data += del;
    }

    template <typename T>
    class SequenceIterator {
    public:

        SequenceIterator(const Sequence<T>& start, int niter, int inc)
            :m_data(start), m_end(niter), m_inc(inc), m_pos(0) {
        }

        bool hasNext()const {
            return m_pos < m_end;
        }

        Sequence<T>& next() {
            m_pos++;
            m_data.slide(m_inc);
            return m_data;
        }

        void reset(int newpos) {
            int del = m_pos - newpos;
            if (del != 0) {
                del *= m_inc;
                m_data.slide(-del);
                m_pos = newpos;
            }
        }

        void begin() {
            reset(0);
        }

    private:

        Sequence<T> m_data;
        int m_end, m_inc, m_pos;

    };


    template <typename T>
    class DataBlock
    {
    public:

        DataBlock(int n) {
            m_data = new T[n];
            m_size = n;
        }

        DataBlock(int n, T* px);

        DataBlock(int n, std::function<T(int)> fn);

        DataBlock(const DataBlock<T>& x);

        DataBlock<T>& operator=(const DataBlock<T>& x);

        Sequence<T> all() {
            return Sequence<T>(m_data, m_size);
        }

        int length() {
            return m_size;
        }

        void rand();

        virtual ~DataBlock();

        template<typename S>
        friend std::ostream& operator<< (std::ostream& stream, DataBlock<S>& seq);


    private:

        T* m_data;
        int m_size;
    };

    template<typename T>
    DataBlock<T>::DataBlock(int n, T* px) {
        m_data = new T[n];
        m_size = n;
        for (int u = 0; u < m_size; ++u) {
            m_data[u] = px[u];
        }
    }

    template<typename T>
    DataBlock<T>::DataBlock(int n, std::function<T(int)> fn) {
        m_data = new T[n];
        m_size = n;
        for (int u = 0; u < m_size; ++u) {
            m_data[u] = fn(u);
        }
    }

    template<typename T>
    DataBlock<T>::DataBlock(const DataBlock<T>& x) {
        m_data = new T[x.m_size];
        m_size = x.m_size;
        for (int u = 0; u < m_size; ++u) {
            m_data[u] = x.m_data[u];
        }
    }

    template<typename T>
    DataBlock<T>& DataBlock<T>::operator=(const DataBlock<T>& x) {
        if (this != &x) {
            delete[] m_data;
            m_data = new T[x.m_size];
            m_size = x.m_size;
            for (int u = 0; u < m_size; ++u) {
                m_data[u] = x.m_data[u];
            }
        }
        return this;
    }


    template<typename T>
    DataBlock<T>::~DataBlock() {
        delete[] m_data;
    }

    template<typename T>
    inline void Sequence<T>::copy(Sequence<T> src)
    {
        T* s = src.m_data;
        int incs = src.m_inc, imax = m_n * m_inc;
        for (int i = 0, j = 0; i != imax; i += m_inc, j += incs) {
            m_data[i] = s[j];
        }
    }

    template<typename T>
    void NUMCPP::Sequence<T>::copyTo(T* buffer) {
        int imax = m_n * m_inc;
        for (int i = 0, j = 0; i != imax; i += m_inc, ++j) {
            buffer[j] = m_data[i];
        }
    }


    template<typename T>
    std::ostream& operator<< (std::ostream& stream, DataBlock<T>& data) {
        return stream << data.all();
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& stream, Sequence<T> seq) {
        if (seq.isEmpty())
            return stream;
        const T* p = seq.begin();
        int inc = seq.increment();
        stream << p[0];
        int imax = seq.length() * inc;
        for (int i = inc; i != imax; i += inc)
            stream << '\t' << p[i];
        return stream;
    }

    template<>
    inline void DataBlock<double>::rand() {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0, 1);
        for (int u = 0; u < m_size; ++u)
            m_data[u] = dist(mt);
    }

    template<typename T>
    void Sequence<T>::mul(int n, T value, T* x, int incx) {
        if (value == NUMCPP::CONSTANTS<T>::one)
            return;
        if (value == NUMCPP::CONSTANTS<T>::zero) {
            set(n, value, x, incx);
            return;
        }
        int imax = incx * n;
        for (int i = 0; i != imax; i += incx)
            x[i] *= value;
    }

    template<typename T>
    void Sequence<T>::add(int n, T value, T* x, int incx) {
        if (value == NUMCPP::CONSTANTS<T>::zero)
            return;
        int imax = incx * n;
        for (int i = 0; i != imax; i += incx)
            x[i] += value;
    }

    template<typename T>
    void Sequence<T>::set(int n, T value, T* x, int incx) {
        int imax = incx * n;
        for (int i = 0; i != imax; i += incx)
            x[i] = value;
    }

    template<typename T>
    void Sequence<T>::addAY(T a, Sequence<T> Y) {
        if (a == NUMCPP::CONSTANTS<T>::zero)
            return;
        int incy = Y.increment();
        T* y = Y.begin();
        int imax = m_inc * m_n;
        for (int i = 0, j = 0; i != imax; i += m_inc, j += incy)
            m_data[i] += a * y[j];
    }
}


#endif
