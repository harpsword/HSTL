//
// Created by harpsword on 2020/4/20.
//

#ifndef STL_VECTOR_H
#define STL_VECTOR_H

#include "stdio.h"

#include "allocator.h"
#include "algorithm.h"
#include "UninitializedFunctions.h"

namespace HSTL{
    template <class T, class Alloc=allocator<T>>
    class vector{
    public:
        using iterator = T*;
        using const_iterator = const T*;

        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
    private:
        static const size_type GrowthRate{2};
        static const size_type InitialSize{8};

        T *_start = nullptr;
        T *_end = nullptr;
        T *_endOfStorage = nullptr;
        using dataAllocator = Alloc;
    public:
        // 构造、析构、复制构造、复制拷贝、移动构造拷贝
        vector();
        vector(size_type n, value_type v);
        // initilize_list
//        vector(initilize_list);
        ~vector();
//        vector& operator = (const vector& v);


        // iterator
        iterator begin() {return _start;}
        iterator end() {return _end;}
        const_iterator cbegin() const {return _start; }
        const_iterator cend() const {return _end; }

        // 容量相关
        difference_type size() const { return _end - _start;}
        difference_type capacity() const { return _endOfStorage - _start; }
        bool empty() const { return _start == _end; }

        // 访问元素
        reference operator[] (const difference_type i) {
            if (i >= size() || i < 0) {
                printf("wrong index %d for size %d", i, size());
                throw 100;
            }
            return *(_start + i);
        }

        // 修改容器相关的操作
        void push_back(T& v);
        void push_back(T&& v);
        iterator insert(iterator pos, const T& value);
        iterator insert(iterator pos, T&& value);

        iterator insert(iterator pos, size_type count, const_reference value);

        iterator insert(iterator pos, iterator first, iterator last);

    private:
        void allocateAndFillN(const size_type n, const value_type& v);
//        void allocateAndCopy()
        void reallocateAndFillN_pos(iterator pos, size_type n, const_reference value);

        void reallocateAndCopy_pos(iterator pos, iterator first, iterator last);
        void destroyAndDeallocateAll();

    };
}

#include "../detail/vector.tpp"
#endif //STL_VECTOR_H
