//
// Created by harpsword on 2020/4/20.
//

#include <iostream>
#include <vector>


#define private public
#include "include/allocator.h"

void output_free_list(int T_size){
    int count{0};
    HSTL::alloc::obj *current_obj = HSTL::alloc::free_list[T_size / 8 - 1];
    while (current_obj != nullptr){
//        std::cout << count << "-th obj addr:" << current_obj << std::endl;
        count ++;
        current_obj = current_obj->next;
    }
    std::cout << "the size of free_list in index " << T_size / 8 - 1 << " is " << count << std::endl;
}

// 测试分配的20个大小为T_size的空间起始地址是否正确
void test_alloc_refill1(){
    size_t T_size = 32;
    char *a = (char *)HSTL::alloc::allocate(T_size);
    std::cout << "the addr of a :" << (void *)(a) << std::endl;
    int count{0};
    HSTL::alloc::obj *current_obj = HSTL::alloc::free_list[T_size / 8 - 1];
    while (current_obj != nullptr){
        std::cout << count << "-th obj addr:" << current_obj << std::endl;
        count ++;
        current_obj = current_obj->next;
    }
    std::cout << "the size of free_list in index " << T_size / 8 - 1 << " is " << count << std::endl;
}

void test_alloc_refill2(){
    size_t nobjs = 10;
    char *a = HSTL::alloc::chunk_alloc(32, nobjs);
    std::cout << "heap size:" << HSTL::alloc::heap_size << std::endl;
    std::cout << "end - start:" << HSTL::alloc::end_free - HSTL::alloc::start_free << std::endl;
    std::cout << "---------------" << std::endl;

    nobjs = 5;
    char *b = HSTL::alloc::chunk_alloc(64, nobjs);
    std::cout << "heap size:" << HSTL::alloc::heap_size << std::endl;
    std::cout << "end - start:" << HSTL::alloc::end_free - HSTL::alloc::start_free << std::endl;
    std::cout << "---------------" << std::endl;

    nobjs = 10;
    char *c = HSTL::alloc::chunk_alloc(72, nobjs);
    std::cout << "heap size:" << HSTL::alloc::heap_size << std::endl;
    std::cout << "end - start:" << HSTL::alloc::end_free - HSTL::alloc::start_free << std::endl;
    std::cout << "---------------" << std::endl;

    /* correct output:
    heap size:640
    end - start:320
    ---------------
    heap size:640
    end - start:0
    ---------------
    heap size:2120
    end - start:760
    ---------------
     */
}

