#ifndef __sequence_h
#define __sequence_h

#include <functional>
#include <algorithm>
#include <random>

#include "constants.h"

namespace NUMCPP {

    template <typename T>
    class Sequence
    {
    public:

        Sequence(T* p0, T* p1, size_t inc) {
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

        Sequence(T* p0, size_t n, size_t inc) {
            this->m_p0 = p0;
            this->m_p1 = p0 + n * inc;
            this->m_inc = inc;
            this->m_size = n;
        }

        Sequence(T* p0, size_t n) {
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

        T operator()(size_t idx)const {
            return *(m_p0 + idx * m_inc);
        }

        T& operator()(size_t idx) {
            return *(m_p0 + idx * m_inc);
        }

        double dot(const Sequence& x)const;

        size_t length()const {
            return m_size;
        }

        size_t increment()const {
            return m_inc;
        }

        void copy(const Sequence& src);

        void copyTo(T* buffer);

        void rand();

        void slide(size_t del);

        template<typename S>
        friend std::ostream& operator<< (std::ostream& stream, const Sequence<S>& seq);

    private:

        T* m_p0, * m_p1;
        size_t m_inc;
        size_t m_size;

    };

    template <typename T>
    inline void Sequence<T>::slide(size_t n) {
        size_t del = m_inc == 1 ? n : n * m_inc;
        m_p0 += del;
        m_p1 += del;
    }

    template <typename T>
    class DataBlock
    {
    public:

        DataBlock(size_t n) {
            m_data = new T[n];
            m_size = n;
        }

        DataBlock(size_t n, T* px);

        DataBlock(const DataBlock<T>& x);

        DataBlock<T>& operator=(const DataBlock<T>& x);

        Sequence<T> all() {
            return Sequence<T>(m_data, m_size);
        }

        const Sequence<T> all()const {
            return Sequence<T>(m_data, m_size);
        }

        size_t length() {
            return m_size;
        }

        void rand();

        virtual ~DataBlock();

        template<typename S>
        friend std::ostream& operator<< (std::ostream& stream, const DataBlock<S>& seq);


    private:

        T* m_data;
        size_t m_size;
    };

    template<typename T>
    DataBlock<T>::DataBlock(size_t n, T* px) {
        m_data = new T[n];
        m_size = n;
        for (size_t u = 0; u < m_size; ++u) {
            m_data[u] = px[u];
        }
    }

    template<typename T>
    DataBlock<T>::DataBlock(const DataBlock<T>& x) {
        m_data = new T[x.m_size];
        m_size = x.m_size;
        for (size_t u = 0; u < m_size; ++u) {
            m_data[u] = x.m_data[u];
        }
    }

    template<typename T>
    DataBlock<T>& DataBlock<T>::operator=(const DataBlock<T>& x) {
        if (this != &x) {
            delete[] m_data;
            m_data = new T[x.m_size];
            m_size = x.m_size;
            for (size_t u = 0; u < m_size; ++u) {
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
        size_t inc = seq.increment();

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
        for (size_t u = 0; u < m_size; ++u)
            m_data[u] = dist(mt);
    }

}


#endif
