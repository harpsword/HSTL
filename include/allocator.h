//
// Created by harpsword on 2020/4/20.
//

#ifndef STL_ALLOCATOR_H
#define STL_ALLOCATOR_H

#include <new> // for placement new
#include <cstddef> // for ptrdiff_t, size_t
#include <cstdlib> // for exit()
#include <climits> // for unit_max
#include <iostream> // for cerr

#include "alloc.h"

namespace HSTL{

    template<class T>
    class allocator{
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

    public:
        static T *allocate();
        static T *allocate(size_t n);
        static void deallocate(T *ptr);
        static void deallocate(T *ptr, size_t n);

        static void construct(T *ptr);
        static void construct(T *ptr, const T& value);
        static void destroy(T *ptr);
        static void destroy(T *first, T *last);
    };
    template<class T>
    T *allocator<T>::allocate(){
        return static_cast<T *>(alloc::allocate(sizeof(T)));
    }
    template<class T>
    T *allocator<T>::allocate(size_t n){
        if (n == 0) return 0;
        return static_cast<T *>(alloc::allocate(sizeof(T)*n));
    }
    template<class T>
    void allocator<T>::deallocate(T *ptr){
        alloc::deallocate(static_cast<void *>(ptr), sizeof(T));
    }
    template <class T>
    void allocator<T>::deallocate(T *ptr, size_t n) {
        if (n==0) return;
        alloc::deallocate(static_cast<void *>(ptr), sizeof(T)*n);
    }
    template <class T>
    void allocator<T>::construct(T *ptr) {
        new(ptr) T();
    }
    template <class T>
    void allocator<T>::construct(T *ptr, const T &value) {
        new(ptr) T(value);
    }
    template <class T>
    void allocator<T>::destroy(T *ptr) {
        ptr->~T();
    }
    template <class T>
    void allocator<T>::destroy(T *first, T *last) {
        for (; first != last; first++)
            first->~T();
    }


}


#endif //STL_ALLOCATOR_H
