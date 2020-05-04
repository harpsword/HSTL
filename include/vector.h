//
// Created by harpsword on 2020/4/20.
//

#ifndef STL_VECTOR_H
#define STL_VECTOR_H

#include "stdio.h"

#include "exception.h"
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
        static const size_type InitialSize{0};

        T *_start = nullptr;
        T *_end = nullptr;
        T *_endOfStorage = nullptr;
        using dataAllocator = Alloc;
    public:
        // 构造、析构、复制构造、复制拷贝、移动构造拷贝
        vector();
        vector(size_type n, value_type v);
        vector(size_type n);
        vector(iterator first, iterator last);
        vector(const vector& other);
        vector(vector&& other) noexcept;
        vector(std::initializer_list<T> init);
        ~vector();
        // operator
        vector& operator= (const vector& other);
        vector& operator= (vector&& other);
        vector& operator= (std::initializer_list<T> init);
        void assign(size_type count, const_reference value);
        void assign(iterator first, iterator last);
        void assign(std::initializer_list<T> init);

        // iterator
        iterator begin() {return _start;}
        iterator end() {return _end;}
        const_iterator cbegin() const {return _start; }
        const_iterator cend() const {return _end; }

        // 容量相关
        bool empty() const { return _start == _end; }
        size_type size() const noexcept { return _end - _start;}
        size_type capacity() const { return _endOfStorage - _start; }
        void reserve(size_type new_cap); // 扩容到 new_cap大小
        void shrink_to_fit();  // 缩减多余空间到 _end=_endOfStorage

        // 访问元素
        reference operator[] (size_type pos) {
            range_check(pos);
            return *(_start + pos);
        }
        const_reference operator[] (size_type pos) const {
            range_check(pos);
            return *(_start + pos);
        }
        reference at(size_type pos) {
            range_check(pos);
            return *(_start + pos);
        }
        const_reference at(size_type pos) const {
            range_check(pos);
            return *(_start + pos);
        }
        reference front(){ return *_start; }
        const_reference front() const { return *_start; }
        reference back() { return *(_end - 1); }
        const_reference back() const { return *(_end-1); }
        constexpr pointer data() noexcept {return _start;}
        constexpr const_pointer data() const noexcept {return _start;}

        // 修改容器相关的操作
        void clear() noexcept;
        iterator insert(iterator pos, const T& value);
        iterator insert(iterator pos, T&& value);
        iterator insert(iterator pos, size_type count, const_reference value);
        iterator insert(iterator pos, iterator first, iterator last);
//        void emplace(); // 在pos这个位置上新建元素，关键在于如何传递参数
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        void push_back(T& v);
        void push_back(T&& v);
        // void emplace_back();
        void pop_back() { if (size()>=1) _end--;}
        // resize vector, if size() > count, delete extra elements
        // else: let size()=count, and append T() or value.
        void resize(size_type count);
        void resize(size_type count, const value_type& value);
        // swap elements with other vector
        void swap(vector& other) noexcept ;

    private:
        void allocateAndFillN(const size_type n, const value_type& v);
        void allocateAndCopy(iterator first, iterator last);
        void reallocateAndFillN_pos_aux(iterator pos, size_type n, const_reference value, size_type new_cap);
        void reallocateAndFillN_pos(iterator pos, size_type n, const_reference value);
        void reallocateAndCopy_pos(iterator pos, iterator first, iterator last);
        void destroyAndDeallocateAll();
        void range_check(size_type pos){
            if (pos >= size() || pos < 0) {
                char *s ="wrong index %d for size %d", pos, size();
                throw out_of_range(s);
            }
        }

    };
}

#include "../detail/vector.tpp"

#endif //STL_VECTOR_H
