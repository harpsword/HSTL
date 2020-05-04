//
// Created by harpsword on 2020/5/3.
//

#ifndef STL_TEST_SKIPLIST_H
#define STL_TEST_SKIPLIST_H

#define private public

#include <iostream>
#include "include/allocator.h"
#include "include/skiplist.h"
#include "test_alloc.h"



void test_skiplistNode(){

    HSTL::skiplist<int> array;
    HSTL::vector<int> key{1};
    for (int i: key)
        array.insertKey(i);

    array.output();


    for (int i = 8; i <= 128; i+= 8)
    output_free_list(i);



}

#endif //STL_TEST_SKIPLIST_H
