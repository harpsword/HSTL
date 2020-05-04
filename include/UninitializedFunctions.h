//
// Created by harpsword on 2020/4/21.
//

#ifndef STL_UNINITIALIZEDFUNCTIONS_H
#define STL_UNINITIALIZEDFUNCTIONS_H

#include <cstddef>
#include <memory.h>
#include "algorithm.h"
#include "TypeTraits.h"

namespace HSTL{
    // fill vector[start: start+n] with v
    template <typename iterator, typename Size, typename T>
    iterator _uninitialized_fill_n(iterator start, Size n, const T& v, _true_type);
    template <typename iterator, typename Size, typename T>
    iterator _uninitialized_fill_n(iterator start, Size n, const T& v, _false_type);

    template <typename iterator, typename Size, typename T>
    iterator uninitialized_fill_n(iterator start, Size n, const T& v){
        typedef typename _type_traits<T>::is_POD_type isPodType;
        return _uninitialized_fill_n(start, n, v, isPodType());
//        if (sizeof(T) == 1) return _uninitialized_fill_n(start, n, v, _true_type());
//        else return _uninitialized_fill_n(start, n, v, _false_type());
    }
    template <typename iterator, typename Size, typename T>
    iterator _uninitialized_fill_n(iterator start, Size n, const T& v, _true_type){
        // for
        memset(static_cast<void *>(start), static_cast<unsigned char>(v), n);
        return start + n;
    }
    template <typename iterator, typename Size, typename T>
    iterator _uninitialized_fill_n(iterator start, Size n, const T& v, _false_type){
        for (Size iter = 0; iter != n; iter++){
            construct((start+iter), v);
        }
        return start + n;
    }

    // copy vector[start:end] to [new_start:new_start+size]
    template <typename iterator>
    iterator _uninitialized_copy_n_1bytes(iterator start, iterator end, iterator new_start);

    template <typename iterator>
    iterator _uninitialized_copy_n_nbytes(iterator start, iterator end, iterator new_start);

    template <typename iterator>
    iterator uninitialized_copy_n(iterator start, iterator end, iterator new_start){
//        auto *i = start;
        if (sizeof(*start) == 1) return _uninitialized_copy_n_1bytes(start, end, new_start);
        else return _uninitialized_copy_n_nbytes(start, end, new_start);
    }

    template <typename iterator>
    iterator _uninitialized_copy_n_1bytes(iterator start, iterator end, iterator new_start){
        memcpy(static_cast<void *>(new_start), static_cast<void *>(start), end - start);
        ptrdiff_t diff = end - start;
        return new_start + diff;
    }

    template <typename iterator>
    iterator _uninitialized_copy_n_nbytes(iterator start, iterator end, iterator new_start){
        for (; start != end; start++){
            construct(new_start, *start);
            new_start++;
        }
        return new_start;
    }

}

#endif //STL_UNINITIALIZEDFUNCTIONS_H
