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

        Sequence() :m_p0(nullptr), m_p1(nullptr), m_inc(0), m_size(0) {

        }

        Sequence(T* p0, T* p1, int inc) :m_p0(p0), m_p1(p1), m_inc(inc) {
            m_size = (p1 - p0) / inc;
        }

        Sequence(T* p0, T* p1) :m_p0(p0), m_p1(p1), m_inc(1), m_size(p1 - p0) {
        }

        Sequence(T* p0, int n, int inc) :m_p0(p0), m_p1(p0 + n * inc), m_inc(inc), m_size(n) {
        }

        Sequence(T* p0, int n) :m_p0(p0), m_p1(p0 + n), m_inc(1), m_size(n) {
        }

        bool isEmpty() {
            return m_size == 0;
        }

        T* begin() {
            return m_p0;
        }

        T* end() {
            return m_p1;
        }

        Sequence<T> left(int n) {
            return Sequence<T>(m_p0, m_p0 + n*m_inc, m_inc, n);
        }

        Sequence<T> right(int n) {
            return Sequence<T>(m_p1 - m_inc*n, m_p1, m_inc, n);
        }

        Sequence<T> drop(int nl, int nr) {
            int nc = nl + nr;
            if (nc >= m_size)
                return Sequence();
            return Sequence<T>(m_p0 + m_inc * nl, m_p1-m_inc*nr, m_inc, n-nc);
        }

        Sequence<T> extract(int start, int n) {
            int nc = start + n;
            if (nc >= m_size)
                return Sequence();
            return Sequence<T>(m_p0 + m_inc * start, n);
        }

        Sequence<T> reverse() {
            return Sequence<T>(m_p1 - m_inc, m_p0 - m_inc, -m_inc, m_size);
        }

        T& operator()(int idx) {
            return *(m_p0 + idx * m_inc);
        }

        void copy(Sequence<T>& src);

        void rand();

        int length() {
            return m_size;
        }

        int increment() {
            return m_inc;
        }

        void copyTo(T* buffer);

        void slide(int del);

        template<typename S>
        friend std::ostream& operator<< (std::ostream& stream, Sequence<S>& seq);

    private:

        T* m_p0, * m_p1;
        int m_inc;
        int m_size;

    };


    template <typename T>
    inline void Sequence<T>::slide(int del) {
        m_p0 += del;
        m_p1 += del;
    }

    template <typename T>
    class SequenceIterator {
    public:

        SequenceIterator(const Sequence<T>& start, int niter, int inc):m_data(start) {
            m_data = start;
            m_end = niter;
            m_inc = inc;
            m_pos = 0;
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
        friend std::ostream& operator<< (std::ostream& stream, const DataBlock<S>& seq);


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
    inline void Sequence<T>::copy(Sequence<T>& src)
    {
        T* psrc = src.m_p0, * ptarget = Sequence<T>::m_p0;
        while (ptarget != Sequence<T>::m_p1) {
            *ptarget = *psrc;
            ptarget += Sequence<T>::m_inc;
            psrc += src.m_inc;
        }
    }

    template<typename T>
    void NUMCPP::Sequence<T>::copyTo(T* buffer) {
        T* psrc = m_p0, * ptarget = buffer;
        while (psrc != m_p1) {
            *ptarget++ = *psrc;
            psrc += m_inc;
        }
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& stream, const DataBlock<T>& data) {
        return stream << data.all();
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& stream, Sequence<T>& seq) {
        if (seq.isEmpty())
            return stream;
        const T* p = seq.begin(), * p1 = seq.end();
        int inc = seq.increment();

        stream << *p;
        p += inc;
        while (p != p1) {
            stream << '\t' << *p;
            p += inc;
        }
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

}


#endif
