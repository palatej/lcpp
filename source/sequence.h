#ifndef __sequence_h
#define __sequence_h

#include <functional>
#include <algorithm>
#include <random>
#include <stdexcept>

#include "constants.h"

namespace NUMCPP {

    template <typename T>
    class Sequence
    {
    public:

        Sequence(T* p0, T* p1, int inc) {
            this->m_p0 = p0;
            this->m_p1 = p1;
            this->m_inc = inc;
            this->m_size = (p1 - p0) / inc;
        }

        Sequence(T* p0, T* p1) {
            this->m_p0 = p0;
            this->m_p1 = p1;
            this->m_inc = 1;
            this->m_size = p1 - p0;
        }

        Sequence(T* p0, int n, int inc) {
            this->m_p0 = p0;
            this->m_p1 = p0 + n * inc;
            this->m_inc = inc;
            this->m_size = n;
        }

        Sequence(T* p0, int n) {
            this->m_p0 = p0;
            this->m_p1 = p0 + n;
            this->m_inc = 1;
            this->m_size = n;
        }

        bool isEmpty()const {
            return m_size == 0;
        }

        T* begin() {
            return m_p0;
        }

        T* end() {
            return m_p1;
        }

        const T* begin()const {
            return m_p0;
        }

        const T* end()const {
            return m_p1;
        }

        Sequence<T> reverse() {
            return Sequence<T>(m_p1-m_inc, m_p0-m_inc, -m_inc);
        }

        const Sequence<T> reverse()const {
            return Sequence<T>(m_p1 - m_inc, m_p0 - m_inc, -m_inc);
        }

        T operator()(int idx)const {
            return *(m_p0 + idx * m_inc);
        }

        T& operator()(int idx) {
            return *(m_p0 + idx * m_inc);
        }

        double dot(const Sequence& x)const;

        int length()const {
            return m_size;
        }

        int increment()const {
            return m_inc;
        }

        void copy(const Sequence& src);

        void copyTo(T* buffer);

        void rand();

        void slide(int del);

        template<typename S>
        friend std::ostream& operator<< (std::ostream& stream, const Sequence<S>& seq);

    private:

        T* m_p0, * m_p1;
        int m_inc;
        int m_size;

    };

    template <typename T>
    inline void Sequence<T>::slide(int n) {
        int del = m_inc == 1 ? n : n * m_inc;
        m_p0 += del;
        m_p1 += del;
    }

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

        const Sequence<T> all()const {
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
    inline void Sequence<T>::copy(const Sequence<T>& src)
    {
        T* psrc = src.m_p0, * ptarget = m_p0;
        while (ptarget != m_p1) {
            *ptarget = *psrc;
            ptarget += m_inc;
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
    std::ostream& operator<< (std::ostream& stream, const Sequence<T>& seq) {
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
