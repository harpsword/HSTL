//
// Created by harpsword on 2020/4/21.
//

#ifndef STL_ALGORITHM_H
#define STL_ALGORITHM_H

namespace HSTL{
    template <class T>
    const T& max(const T& a, const T& b){
        return (a > b) ? a : b;
    }

    template<typename T1, typename T2>
    inline void construct(T1 *ptr1, const T2& value){
        new(ptr1) T1(value);
    }

}

#endif //STL_ALGORITHM_H
