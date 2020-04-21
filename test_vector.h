//
// Created by harpsword on 2020/4/21.
//

#ifndef STL_TEST_VECTOR_H
#define STL_TEST_VECTOR_H

#define private public
#include "include/vector.h"
#include "test_alloc.h"

void output_array(HSTL::vector<int> &v){
    std::cout << "size of vector:" << v.size() << std::endl;
    std::cout << "capacity of vector:" << v.capacity() <<std::endl;
    for (auto i = v.begin(); i!=v.end(); i++){
        std::cout << *i << " " ;
    }
    std::cout << std::endl << "---------" << std::endl;
}

void test_vector_init(){
    HSTL::vector<int> array;
    array.push_back(5);

    std::cout << "Static int value:" << HSTL::vector<int>::InitialSize << std::endl;
    std::cout << " start " << array._start << std::endl;
    std::cout << " end " << array._end << std::endl;
    std::cout << "end of Storage: " << array._endOfStorage << std::endl;

    std::cout << "int size:" << sizeof(int) << std::endl;
    // 测试空间配置器那的内存链接
    std::cout << "address of 32 bytes list :" << HSTL::alloc::free_list[3] << std::endl;
    // output freelist description
    output_free_list(32);
}

void test_vector_init_n_v(){
    HSTL::vector<int> v1(10, 5);
    for (auto iter = v1.begin(); iter != v1.end(); iter++)
        std::cout << *iter << " ";
    std::cout << std::endl;
    for (int i = 8; i <= 128; i+=8)
        output_free_list(i);

    HSTL::vector<char> v2(10, 'a');
    for (auto iter = v2.begin(); iter != v2.end(); iter++)
        std::cout << *iter << " ";
    std::cout << std::endl;
    for (int i = 8; i <= 128; i+=8)
        output_free_list(i);
}

void test_vector_iterator(){
    HSTL::vector<int> array;
    int a1[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i != 5; i++)
        array.push_back(a1[i]);
    // output value
    for (auto iter = array.begin(); iter != array.end(); iter++){
        std::cout << *iter << " ";
    }
}

void test_vector_index_push(){
    HSTL::vector<int> array;
    int a1[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i != 5; i++)
        array.push_back(a1[i]);
    std::cout << "index:" << 3 << " ; value:" << array[3] << std::endl;
    for (int i = 0; i != 5; i++)
        std::cout << "using index to visit: " << array[i] << std::endl;
}

void test_vector_insert(){
    HSTL::vector<int> array(3, 5);
    output_array(array);

    auto iter1 = array.insert(array.begin(), 0);
    output_array(array);



}

#endif //STL_TEST_VECTOR_H
