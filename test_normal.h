//
// Created by harpsword on 2020/4/21.
//

#ifndef STL_TEST_NORMAL_H
#define STL_TEST_NORMAL_H

#include <iostream>

void output_addr(void *v){
    std::cout << "addr:" << v << std::endl;
}

void test_const_1(){
    int i = 42;
    const int &r1 = i;
    const int &r2 = 42;
    const int &r3 = r1 * 2;

    output_addr((void *) &r1);
    output_addr((void *) &r2);
    output_addr((void *) &r3);

    //---output----
    // r1和i的地址相同，说明采用相同的地址
//    addr:0x7ffee8f948dc
//    addr:0x7ffee8f948c4  地址不同，新采用了一个内存空间
//    addr:0x7ffee8f948b4

    int &ri = i;
    std::cout << "-----output reference to int-----" << std::endl;
    output_addr((void *) &i);
    output_addr((void *) &ri);
//  ----output----
//    output reference to int
//    addr:0x7ffee8f948dc
//    addr:0x7ffee8f948dc
}

void test_static_cast(){
    const char &a = 'a';

    char b = static_cast<char>(a);
    std::cout << b << std::endl;
}

void test_pointer(){
    //    int * a = new int(5);
    char * a = new char('a');
    std::cout << sizeof(a) << std::endl;
    std::cout << sizeof(*a) << std::endl;
}

#endif //STL_TEST_NORMAL_H
