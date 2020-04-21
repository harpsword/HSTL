//
// Created by harpsword on 2020/4/21.
//

#ifndef STL_UNINITIALIZEDFUNCTIONS_H
#define STL_UNINITIALIZEDFUNCTIONS_H

#include <cstddef>
#include <memory.h>
#include "algorithm.h"

namespace HSTL{
    // fill vector[start: start+n] with v
    template <typename iterator, typename Size, typename T>
    iterator _uninitialized_fill_n_1bytes(iterator start, Size n, const T& v);
    template <typename iterator, typename Size, typename T>
    iterator _uninitialized_file_n_nbyets(iterator start, Size n, const T& v);
    template <typename iterator, typename Size, typename T>
    iterator uninitialized_fill_n(iterator start, Size n, const T& v){
        if (sizeof(T)==1) return _uninitialized_fill_n_1bytes(start, n, v);
        else return _uninitialized_file_n_nbyets(start, n, v);
    }
    template <typename iterator, typename Size, typename T>
    iterator _uninitialized_fill_n_1bytes(iterator start, Size n, const T& v){
        memset(static_cast<void *>(start), static_cast<char>(v), n);
        return start + n;
    }
    template <typename iterator, typename Size, typename T>
    iterator _uninitialized_file_n_nbyets(iterator start, Size n, const T& v){
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
        auto i = start;
        if (sizeof(i) == 1) return _uninitialized_copy_n_1bytes(start, end, new_start);
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
