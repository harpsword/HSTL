//
// Created by harpsword on 2020/5/4.
//

#ifndef STL_MAP_H
#define STL_MAP_H

#include <functional>
#include "allocator.h"
#include "skiplist.h"

namespace HSTL{
    template<
        typename Key,
        typename T,
        typename Compare=std::less<Key>>
    class map{
    public:
        using key_value = Key;
        using value_type = T;

    private:
        skiplist<Key, T> data;

    public:
        map();
        ~map();


    };
}

#endif //STL_MAP_H
