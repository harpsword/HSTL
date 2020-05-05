//
// Created by harpsword on 2020/5/5.
//

#ifndef STL_FUNCTIONAL_H
#define STL_FUNCTIONAL_H

#include "utility"

namespace HSTL{

    template<typename T>
    struct less{
        constexpr bool operator()(T &lhs, T &rhs) const {
            return lhs < rhs;
        }
    };

    template<typename T>
    struct greater{
        constexpr bool operator()(T &lhs, T &rhs) const{
            return lhs > rhs;
        }
    };

}

#endif //STL_FUNCTIONAL_H
