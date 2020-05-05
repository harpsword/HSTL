//
// Created by harpsword on 2020/5/5.
//

#ifndef STL_TEST_MAP_H
#define STL_TEST_MAP_H

#include <map>
#include "include/map.h"
#include "Profiler/Profiler.h"

void test_map_init(){
    HSTL::map<int, int> data;
    HSTL::vector<int> key{1, 2, 2, 2, 100, 99, 88};
    HSTL::vector<int> value{1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i != key.size(); i++){
        data[key[i]] = value[i];
    }

    for (auto iter : data){
        std::cout << iter.first << " " << iter.second << std::endl;
    }
}

void test_map_costtime(){
    std::map<int, int> array;
//    HSTL::map<int, int> array;
    TinySTL::Profiler::ProfilerInstance::start();
    int number = 10000000;
    int i = 0;
    for (; i != number; ++i){
        array[i] = i;
    }
    TinySTL::Profiler::ProfilerInstance::finish();
    TinySTL::Profiler::ProfilerInstance::dumpDuringTime();
}

#endif //STL_TEST_MAP_H
