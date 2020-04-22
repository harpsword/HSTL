//
// Created by harpsword on 2020/4/21.
//

#ifndef STL_TEST_VECTOR_H
#define STL_TEST_VECTOR_H

#define private public
#include "include/vector.h"
#include "test_alloc.h"
#include "Profiler/Profiler.h"

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
    HSTL::vector<int> array(8, 5);
    output_array(array);
    auto iter1 = array.insert(array.begin(), 100, 0);
    output_array(array);

    HSTL::vector<int> array2(100, 1);
    iter1 = array.insert(array.begin(), array2.begin(), array2.end());
    output_array(array);

    const int& value = array[1];
    int &value2 = array[2];
}

void test_vector_push_back_timeCost_forint(){
//    std::vector<int> vec;
    HSTL::vector<int> vec;
    TinySTL::Profiler::ProfilerInstance::start();
    int number = 1000000;
    int i = 0;
    for (; i != number; ++i){
        vec.push_back(i);
    }
    TinySTL::Profiler::ProfilerInstance::finish();
    TinySTL::Profiler::ProfilerInstance::dumpDuringTime();
}

void test_vector_shrink(){
    HSTL::vector<int> v;
    std::cout << "Default-constructed capacity is " << v.capacity() << '\n';
    v.resize(100);
    std::cout << "Capacity of a 100-element vector is " << v.capacity() << '\n';
    v.clear();
    std::cout << "Capacity after clear() is " << v.capacity() << '\n';
    v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << v.capacity() << '\n';
    v.push_back(10);
    std::cout << "Capacity after push_back() is " << v.capacity() << '\n';

}

void test_vector_erase(){
    //suggested output
//    0 1 2 3 4 5 6 7 8 9
//    1 2 3 4 5 6 7 8 9
//    1 2 6 7 8 9
//    1 7 9
    HSTL::vector<int> c ={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (auto &i : c) {
        std::cout << i << " ";
    }
    std::cout << '\n';
    c.erase(c.begin());
    for (auto &i : c) {
        std::cout << i << " ";
    }
    std::cout << '\n';

    c.erase(c.begin()+2, c.begin()+5);

    for (auto &i : c) {
        std::cout << i << " ";
    }
    std::cout << '\n';

    // Erase all even numbers (C++11 and later)
    for (auto it = c.begin(); it != c.end(); ) {
        if (*it % 2 == 0) {
            it = c.erase(it);
        } else {
            ++it;
        }
    }

    for (auto &i : c) {
        std::cout << i << " ";
    }
    std::cout << '\n';
}

void display_sizes(const HSTL::vector<int>& nums1,
                   const HSTL::vector<int>& nums2,
                   const HSTL::vector<int>& nums3)
{
    std::cout << " nums1: " << nums1.size()
              << " nums2: " << nums2.size()
              << " nums3: " << nums3.size() << '\n';
}

int test_vector_operator_equal()
{
    HSTL::vector<int> nums1{1, 2, 3, 4, 5};
    HSTL::vector<int> nums2 = {2, 3, 4, 5, 6};
    HSTL::vector<int> nums3;

    std::cout << "Initially:\n";
    display_sizes(nums1, nums2, nums3);

    // copy assignment copies data from nums1 to nums2
    nums2 = nums1;

    std::cout << "After assigment:\n";
    display_sizes(nums1, nums2, nums3);

    std::cout << "before move,addr:" << nums1._start << std::endl;
    // move assignment moves data from nums1 to nums3,
    // modifying both nums1 and nums3
    nums3 = std::move(nums1);

    std::cout << "After move, addr:" << nums3._start << std::endl;
    std::cout << "After move assigment:\n";
    display_sizes(nums1, nums2, nums3);
}

#endif //STL_TEST_VECTOR_H
