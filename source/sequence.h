#ifndef __sequence_h
#define __sequence_h

#include <functional>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <iterator>
#include <cstddef>  
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

        struct ConstIterator {

            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = const T*;
            using reference = const T&;

            ConstIterator(pointer ptr, int inc) :mPtr(ptr), mInc(inc) {}

            pointer operator->()const {
                return mPtr;
            }

            reference operator*()const {
                return *mPtr;
            }

            //++T
            ConstIterator operator++() {
                mPtr += mInc;
                return *this;
            }

            //T++
            ConstIterator operator++(int) {
                ConstIterator tmp = *this;
                mPtr += mInc;
                return tmp;
            }

            friend bool operator==(const ConstIterator& l, const ConstIterator& r) {
                return l.mPtr == r.mPtr;
            }

            friend bool operator!=(const ConstIterator& l, const ConstIterator& r) {
                return l.mPtr != r.mPtr;
            }

        private:

            pointer mPtr;
            int mInc;
        };


        struct Iterator {

            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;


            Iterator(pointer ptr, int inc) :mPtr(ptr), mInc(inc) {}

            pointer operator->()const {
                return mPtr;
            }

            reference operator*()const {
                return *mPtr;
            }

            //++T
            Iterator operator++() {
                mPtr+=mInc;
                return *this;
            }

            //T++
            Iterator operator++(int) {
                Iterator tmp = *this;
                mPtr += mInc;
                return tmp;
            }

            friend bool operator==(const Iterator& l, const Iterator& r) {
                return l.mPtr == r.mPtr;
            }

            friend bool operator!=(const Iterator& l, const Iterator& r) {
                return l.mPtr != r.mPtr;
            }

        private:

            pointer mPtr;
            int mInc;
        };


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

        Iterator begin() {
            return Iterator(m_data, m_inc);
        }

        Iterator end() {
            return Iterator(m_data + m_inc * m_n, m_inc);
        }

        ConstIterator cbegin() const{
            return ConstIterator(m_data, m_inc);
        }

        ConstIterator cend() const {
            return ConstIterator(m_data + m_inc * m_n, m_inc);
        }

        Sequence<T> left(int n) {
            return Sequence<T>(m_data, n, m_inc);
        }

        Sequence<T> right(int n) {

            return Sequence<T>(m_data + m_inc * (m_n - n), n, m_inc);
        }

        Sequence<T> drop(int nl, int nr) {
            int nc = nl + nr;
            if (nc >= m_n)
                return Sequence();
            return Sequence<T>(m_data + m_inc * nl, m_n - nc, m_inc);
        }

        Sequence<T> extend(int nl, int nr)const {
            return drop(-nl, -nr);
        }

        Sequence<T> extract(int start, int n)const {
            int nc = start + n;
            if (nc >= m_n)
                return Sequence();
            return Sequence<T>(m_data + m_inc * start, n, m_inc);
        }

        Sequence<T> reverse() const{
            return Sequence<T>(m_data + (m_n - 1) * m_inc, m_n, -m_inc);
        }

        T& operator()(int idx) {
            return *(m_data + idx * m_inc);
        }

        T operator()(int idx) const {
            return *(m_data + idx * m_inc);
        }

        void copy(Sequence<T> src);

        void rand();

        int length() const {
            return m_n;
        }

        int increment() const{
            return m_inc;
        }

        void copyTo(T* buffer)const;

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

        T asum();

        T sum();
        T sum2()const;

        T sum(std::function<T(T)> fn);

        void apply(std::function<T(T)> fn);

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
        std::copy(src.cbegin(), src.cend(), begin());
    }

    template<typename T>
    void NUMCPP::Sequence<T>::copyTo(T* buffer) const{

        ConstIterator beg = cbegin(), end = cend();
        while (beg != end) {
            *buffer++ = *beg++;
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
        auto p = seq.cbegin(), e=seq.cend();
        stream <<*p++;
        while (p != e)
            stream << '\t' << *p++;
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
        Iterator b = begin(), e = end();
        ConstIterator y = Y.cbegin();
        while (b != e) {
            *b++ += a * *y++;
        }
    }

    template<typename T>
    T Sequence<T>::asum() {
        T asum = 0;
        int imax = m_inc * m_n;
        for (int i = 0; i != imax; i += m_inc) {
            asum += abs(m_data[i]);
        }
        return asum;
    }

    template<typename T>
    T Sequence<T>::sum() {
        T asum = CONSTANTS<T>::zero;
        int imax = m_inc * m_n;
        for (int i = 0; i != imax; i += m_inc) {
            asum += m_data[i];
        }
        return asum;
    }

    template<typename T>
    T Sequence<T>::sum2()const {
        return std::accumulate(cbegin(), cend(), CONSTANTS<T>::zero);
    }

    template<typename T>
    T Sequence<T>::sum(std::function<T(T)> fn) {
        T asum = 0;
        int imax = m_inc * m_n;
        for (int i = 0; i != imax; i += m_inc) {
            asum += fn(m_data[i]);
        }
        return asum;
    }

    template<typename T>
    void Sequence<T>::apply(std::function<T(T)> fn) {
        int imax = m_inc * m_n;
        for (int i = 0; i != imax; i += m_inc) {
            m_data[i] = fn(m_data[i]);
        }
    }


}


#endif
