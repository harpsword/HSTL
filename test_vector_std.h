//
// Created by harpsword on 2020/4/21.
//

#ifndef STL_TEST_VECTOR_STD_H
#define STL_TEST_VECTOR_STD_H

#include <vector>

void test_vector_1(){
    std::vector<int> v;
    auto iter = v.begin();
    std::vector<int>::iterator test_iter = v.begin();
    auto iter2 = v.insert(v.begin(), 1);

}

#endif //STL_TEST_VECTOR_STD_H
