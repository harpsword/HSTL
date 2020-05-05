//
// Created by harpsword on 2020/5/3.
//

#ifndef STL_TEST_SKIPLIST_H
#define STL_TEST_SKIPLIST_H

#define private public

#include <iostream>
#include "include/allocator.h"
#include "include/skiplist.h"
#include "include/utility.h"
#include "test_alloc.h"

void test_skiplistNode(){

    HSTL::skiplist<HSTL::pair<int, int>> array;
    HSTL::vector<int> key{1, 2, 2, 2, 100, 99, 88};
    HSTL::vector<int> value{1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i != key.size(); i++){
        array.insert_multi(HSTL::pair<int, int>{key[i], value[i]});
    }

//    array.output();

    for (auto i : array){
        std::cout << i.first << " " << i.second << std::endl;
    }


    for (int i = 8; i <= 128; i+= 8)
    output_free_list(i);



}

#endif //STL_TEST_SKIPLIST_H
