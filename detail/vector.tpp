//
// Created by harpsword on 2020/4/21.
//#include <iostream>

#include "../include/vector.h"

namespace HSTL{
    template <typename T, typename Alloc>
    const typename vector<T, Alloc>::size_type vector<T, Alloc>::InitialSize;

    template <class T, class Alloc>
    vector<T, Alloc>::vector() {
        allocateAndFillN(0, 0);
    }
    template <class T, class Alloc>
    vector<T, Alloc>::vector(size_type n, value_type v) {
        allocateAndFillN(n, v);
    }

    template <class T, class Alloc>
    vector<T, Alloc>::~vector(){
        dataAllocator::destroy(_start, _end);
        dataAllocator::deallocate(_start, _endOfStorage - _start);
    }

//    template<class T, class Alloc>
//    vector<T, Alloc>& vector<T, Alloc>::operator = (const vector& v){
//        if (this != &v){
//            allocateAndCopy(v.start_, v.finish_);
//        }
//        return *this;
//    }

    template <class T, class Alloc>
    void vector<T, Alloc>::push_back(T &&v) {
        push_back(static_cast<T&>(v));
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::push_back(T &v) {
        iterator tmp = insert(_end, 1, v);
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, T &&value) {
        return insert(pos, static_cast<const T&>(value));
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, const T &value) {
        return insert(pos, 1, value);
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, size_type count, const_reference value) {
        difference_type left_space = _endOfStorage - _end;
        difference_type required_space = count;
        if (left_space >= required_space){
            // move [pos, _end) to [pos+count, _end+count)
            iterator tmpIter = _end - 1;
            for(; tmpIter - pos>=0 ; tmpIter--){
                construct((tmpIter+required_space), *tmpIter);
            }
            iterator tmp = uninitialized_fill_n(pos, required_space, value);
            _end = _end + required_space;
        }else {
            // 空间不够需要重新申请空间
            reallocateAndFillN_pos(pos, count, value);
        }
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, iterator first, iterator last) {
        difference_type left_space = _endOfStorage - _end;
        difference_type required_space = last - first;
        if (left_space >= required_space){
            // move [first, last) to [pos+count, _end+count)
            iterator tmpIter = _end - 1;
            for(; tmpIter - pos>=0 ; tmpIter--){
                construct((tmpIter+required_space), *tmpIter);
            }
            iterator tmp = uninitialized_copy_n(first, last, pos);
            _end = _end + required_space;
        }else {
            reallocateAndCopy_pos(pos, first, last);
        }
        return _start + required_space;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type &v) {
        size_type allocate_size = max(InitialSize, n);
        _start = dataAllocator::allocate(allocate_size);
        _end = uninitialized_fill_n(_start, n, v);
        _endOfStorage = _start + allocate_size;
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::reallocateAndFillN_pos(iterator pos, size_type n, const_reference value) {
        size_type allocate_size = capacity() * GrowthRate;
        pointer newStart = dataAllocator::allocate(allocate_size);
        pointer newEnd = uninitialized_copy_n(_start, pos, newStart);
        newEnd = uninitialized_fill_n(newEnd, n, value);
        newEnd = uninitialized_copy_n(pos, _end, newEnd);

        destroyAndDeallocateAll();
        _start = newStart;
        _end = newEnd;
        _endOfStorage = _start + allocate_size;
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::reallocateAndCopy_pos(iterator pos, iterator first, iterator last) {
        size_type allocate_size = capacity() * GrowthRate;
        pointer newStart = dataAllocator::allocate(allocate_size);
        pointer newEnd = uninitialized_copy_n(_start, pos, newStart);
        newEnd = uninitialized_copy_n(first, last, newEnd);
        newEnd = uninitialized_copy_n(pos, _end, newEnd);

        destroyAndDeallocateAll();
        _start = newStart;
        _end = newEnd;
        _endOfStorage = _start + allocate_size;
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::destroyAndDeallocateAll() {
        dataAllocator::destroy(_start, _end);
        dataAllocator::deallocate(_start, capacity());
    }
}



//

