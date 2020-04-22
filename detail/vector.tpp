//
// Created by harpsword on 2020/4/21.
//#include <iostream>

#include "../include/vector.h"

namespace HSTL{
    template <typename T, typename Alloc>
    const typename vector<T, Alloc>::size_type vector<T, Alloc>::InitialSize;
    template <typename T, typename Alloc>
    const typename vector<T, Alloc>::size_type vector<T, Alloc>::GrowthRate;

    template <class T, class Alloc>
    vector<T, Alloc>::vector() {
        allocateAndFillN(0, 0);
    }

    template <class T, class Alloc>
    vector<T, Alloc>::vector(size_type n, value_type v) {
        allocateAndFillN(n, v);
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(size_type n){
        vector(n, T());
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(iterator first, iterator last) {
        allocateAndCopy(first, last);
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(const vector& other){
        vector(other._start, other._end);
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(vector<T, Alloc> &&other) noexcept {
        _start = other._start;
        _end = other._end;
        _endOfStorage = other._endOfStorage;
        other._start = other._end = other._endOfStorage = nullptr;
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(std::initializer_list<T> init) {
        pointer first = const_cast<pointer>(init.begin());
        pointer last = const_cast<pointer>(init.end());
        allocateAndCopy(first, last);
    }

    template <class T, class Alloc>
    vector<T, Alloc>::~vector(){
        destroyAndDeallocateAll();
    }

    template<class T, class Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator = (const vector& other){
        if (this != &other){
            assign(other._start, other._end);
//            if (capacity() == 0){
//                allocateAndCopy(other._start, other._end);
//            }else if (capacity() < other.size()){
//                 重新分配空间，并复制
//                destroyAndDeallocateAll();
//                allocateAndCopy(other._start, other._end);
//            }else {
//                需要先destroy，再直接复制
//               dataAllocator::destroy(_start, _end);
//               _end = uninitialized_copy_n(other._start, other._end, _start);
//            }
        }
        return *this;
    }

    template<typename T, typename Alloc>
    vector<T, Alloc> & vector<T, Alloc>::operator=(vector<T, Alloc> &&other) {
        destroyAndDeallocateAll();
        _start = other._start;
        _end = other._end;
        _endOfStorage = other._endOfStorage;
        other._start = other._end = other._endOfStorage = nullptr;
        return * this;
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<T> init) {
        assign(init);
        return *this;
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::assign(size_type count, const_reference value) {
        dataAllocator::destroy(_start,_end);
        _end = _start;
        insert(begin(), count, value);
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::assign(iterator first, iterator last) {
        dataAllocator::destroy(_start,_end);
        _end = _start;
        insert(begin(), first, last);
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::assign(std::initializer_list<T> init) {
        dataAllocator::destroy(_start,_end);
        _end = _start;
        iterator first = const_cast<iterator>(init.begin());
        iterator last = const_cast<iterator>(init.end());
        insert(begin(), first, last);
    }

    // 容量相关
    template<typename T, typename Alloc>
    void vector<T, Alloc>::reserve(size_type new_cap) {
        if (capacity() < new_cap){
            pointer newStart = dataAllocator::allocate(new_cap);
            pointer newEnd = uninitialized_copy_n(_start, _end, newStart);
            destroyAndDeallocateAll();
            _start = newStart;
            _end = newEnd;
            _endOfStorage = newStart + new_cap;
        }
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::shrink_to_fit() {
        if (_endOfStorage - _end > 0){
            dataAllocator::deallocate(_end, _endOfStorage - _end);
            _endOfStorage = _end;
        }
    }

    // 修改容器相关的操作
    template<typename T, typename Alloc>
    void vector<T, Alloc>::clear() noexcept {
        dataAllocator::destroy(_start, _end);
        _end = _start;
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
            auto tmp = uninitialized_fill_n(pos, required_space, value);
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

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator pos) {
        return erase(pos, pos+1);
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
        range_check(first - _start);
        range_check(last - _start - 1);
        size_type n = last - first;
        iterator tmpIter = last;
        for (; tmpIter != _end; tmpIter++){
            construct(tmpIter-n, *tmpIter);
        }
        _end -= n;
        dataAllocator::destroy(_end, _end+n);
        return first;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::push_back(T &&v) {
        push_back(static_cast<T&>(v));
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::push_back(T &v) {
        iterator tmp = insert(_end, 1, v);
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_type count) {
        resize(count, T());
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_type count, const value_type &value) {
        if (size() > count){
            _end = _start + count;
        }else {
            if (count < capacity()){
                _end = uninitialized_fill_n(_end, count - size(), value);
            }else {
                reallocateAndFillN_pos_aux(_end, count - size(), value, count);
            }
        }
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::swap(vector<T, Alloc> &other) noexcept {
        if (this != &other){
            swap(_start, other._start);
            swap(_end, other._end);
            swap(_endOfStorage, other._endOfStorage);
        }
    }

    // priavte function
    template <class T, class Alloc>
    void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type &v) {
        size_type allocate_size = max(InitialSize, n);
        _start = dataAllocator::allocate(allocate_size);
        _end = uninitialized_fill_n(_start, n, v);
        _endOfStorage = _start + allocate_size;
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::allocateAndCopy(iterator first, iterator last) {
        size_type difference = last - first;
        size_type allocate_size = max(InitialSize, difference);
        _start = dataAllocator::allocate(allocate_size);
        _end = uninitialized_copy_n(first, last,_start);
        _endOfStorage = _start + allocate_size;
    }

    // 指定了新分配的空间大小，并且保证复制过去的数据顺序为
    // [_start, pos), n * value, [pos, _end)，即有n个数据插入到pos中
    template<typename T, typename Alloc>
    void vector<T, Alloc>::reallocateAndFillN_pos_aux(iterator pos, size_type n, const_reference value, size_type new_cap) {
        size_type allocate_size = new_cap;
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
    void vector<T, Alloc>::reallocateAndFillN_pos(iterator pos, size_type n, const_reference value) {
        size_type allocate_size = (size()+n) * GrowthRate;
        allocate_size = max(allocate_size, InitialSize);
        reallocateAndFillN_pos_aux(pos, n, value, allocate_size);
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::reallocateAndCopy_pos(iterator pos, iterator first, iterator last) {
        size_type allocate_size = (size()+last-first) * GrowthRate;
        allocate_size = max(allocate_size, InitialSize);
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




