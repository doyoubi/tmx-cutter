#ifndef DYB_ARRAY2D
#define DYB_ARRAY2D

#include <iterator>
#include <iostream>
#include <algorithm>
#include <initializer_list>
#include "point2d.h"

namespace dyb
{

    using std::cerr;
    using std::cout;
    using std::endl;
    using std::initializer_list;
    using dyb::point2d;

    template<class T>
    class array2d
    {
    public:
        // iterate column by column !!
        class iterator : public std::iterator<std::forward_iterator_tag, T>
        {
        public:
            iterator(){}
            iterator(T * const _ptr);
            iterator(const iterator & other);
            iterator & operator = (const iterator other);
            const iterator & operator ++ ();
            const iterator operator ++ (int);
            bool operator == (iterator other);
            bool operator != (iterator other);
            T & operator * ();
            T * operator -> ();
            T * get() const;
        private:
            T * ptr;
        };

        point2d<int> iterToPoint(const iterator it) const;

        array2d(int width, int height);

        void set_data(initializer_list<T> list);
        void transpose();

        const T * getRawData() const { return data; }

        T * const operator[](int x);
        const T * const operator[](int x)const;
        T & operator[](point2d<int> point);
        const T & operator[](point2d<int> point)const;

        bool is_valid_position(int x, int y);
        bool is_valid_position(point2d<int> p);
        bool is_border(int x, int y);
        bool is_border(point2d<int> p);
        int get_width()const;
        int get_height()const;

        iterator begin();
        iterator end(); 
        // not implement const_iterator

        array2d(const array2d<T> & other_image);
        array2d & operator = (const array2d<T> & other_image);
        // move
        array2d(array2d<T> && other);
        array2d & operator = (array2d<T> && other_image);

        ~array2d();
    private:
        int width, height;
        T * data;
    };


    // implementation

    // iterator

    template<class T>
    array2d<T>::iterator::iterator(T * const _ptr)
    {
        ptr = _ptr;
    }

    template<class T>
    array2d<T>::iterator::iterator(const typename array2d<T>::iterator & other)
    {
        ptr = other.ptr;
    }

    template<class T>
    typename array2d<T>::iterator & array2d<T>::iterator::operator = (const typename array2d<T>::iterator other)
    {
        this->ptr = other.ptr;
        return *this;
    }

    template<class T>
    const typename array2d<T>::iterator & array2d<T>::iterator::operator ++ ()
    {
        ptr++;
        return *this;
    }

    template<class T>
    const typename array2d<T>::iterator array2d<T>::iterator::operator ++ (int)
    {
        array2d<T>::iterator temp = *this;
        ptr++;
        return temp;
    }

    template<class T>
    bool array2d<T>::iterator::operator == (typename array2d<T>::iterator other)
    {
        return this->ptr == other.ptr;
    }

    template<class T>
    bool array2d<T>::iterator::operator != (typename array2d<T>::iterator other)
    {
        return this->ptr != other.ptr;
    }

    template<class T>
    T & array2d<T>::iterator::operator * ()
    {
        return *ptr;
    }

    template<class T>
    T * array2d<T>::iterator::operator -> ()
    {
        return ptr;
    }

    template<class T>
    T * array2d<T>::iterator::get() const
    {
        return ptr;
    }

    // 

    template<class T>
    void array2d<T>::set_data(initializer_list<T> list)
    {
        if(list.size() != get_width() * get_height())
        {
            std::cerr << "invalid initializer_list size" <<endl;
            exit(1);
        }
        std::copy(list.begin(), list.end(), begin());
    }

    template<class T>
    void array2d<T>::transpose()
    {
        array2d<T> temp(get_height(), get_width());
        for(int y = 0; y < get_height(); y++)
        {
            for(int x = 0; x < get_width(); x++)
            {
                temp[y][x] = (*this)[x][y];
            }
        }
        (*this) = std::move(temp);
    }

    template<class T>
    point2d<int> array2d<T>::iterToPoint(const iterator it) const
    {
        if (begin().get() > it.get() || it.get() >= end().get())
        {
            std::cerr << "invalid iterator" << endl;
            exit(1);
        }
        T * pointer = it.get();
        T * head = begin().get();
        int index = pointer - head; // start from 0
        point2d<int> p;
        p.y = index % height;
        p.x = (index - p.y) / height;
        return p;
    }

    template<class T>
    bool array2d<T>::is_valid_position(int x, int y)
    {
        return 0 <= x && x < width
            && 0 <= y && y < height;
    }

    template<class T>
    bool array2d<T>::is_valid_position(point2d<int> p)
    {
        return is_valid_position(p.x, p.y);
    }

    template<class T>
    bool array2d<T>::is_border(int x, int y)
    {
        return x == 0
            || y == 0
            || x == width - 1
            || y == height - 1;
    }

    template<class T>
    bool array2d<T>::is_border(point2d<int> p)
    {
        return is_border(p.x, p.y);
    }

    template<class T>
    int array2d<T>::get_width()const
    {
        return width;
    }
    template< class T>
    int array2d<T>::get_height()const
    {
        return height;
    }

    template<class T>
    T * const array2d<T>::operator[](int x)
    {
        return data + x * height;
    }

    template<class T>
    const T * const array2d<T>::operator [](int x)const
    {
        return data + x * height;
    }


    template<class T>
    T & array2d<T>::operator [] (point2d<int> point)
    {
        return *(data + point.x * height + point.y);
    }

    template<class T>
    const T & array2d<T>::operator [](point2d<int> point)const
    {
        return *(data + point.x * height + point.y);
    }

    // constructor
    template<class T>
    array2d<T>::array2d(int _width, int _height)
        : width(_width), height(_height), data(nullptr)
    {
        data = new T[width*height];
    }

    template<class T>
    array2d<T>::array2d(const array2d<T> & other_image)
        : width(other_image.width), height(other_image.height)
    {
        data = new T[width * height];
        for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            (*this)[x][y] = other_image[x][y];
    }

    template<class T>
    array2d<T> & array2d<T>::operator = (const array2d<T> & other_image)
    {
        T * temp = data;
        data = new T[other_image.width * other_image.height];
        width = other_image.width;
        height = other_image.height;
        for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            (*this)[x][y] = other_image[x][y];
        delete[] temp;
        return *this;
    }

    // move constructor
    template<class T>
    array2d<T>::array2d(array2d<T> && other_image)
        : width(other_image.width), height(other_image.height), data(nullptr)
    {
        data = other_image.data;
        other_image.data = nullptr;
    }

    // move assignment
    template<class T>
    array2d<T> & array2d<T>::operator = (array2d<T> && other_image)
    {
        T * temp = data;
        data = other_image.data;
        other_image.data = nullptr;
        width = other_image.width;
        height = other_image.height;
        delete temp;
        return (*this);
    }

    template<class T>
    array2d<T>::~array2d()
    {
        if (data) delete[] data;
    }

    template<class T>
    typename array2d<T>::iterator array2d<T>::begin()
    {
        return array2d<T>::iterator(data);
    }

    template<class T>
    typename array2d<T>::iterator array2d<T>::end()
    {
        return array2d<T>::iterator(data + width*height);
    }

}

#endif