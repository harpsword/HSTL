//
// Created by harpsword on 2020/5/5.
//

#ifndef STL_UTILITY_H
#define STL_UTILITY_H

#include <iostream>

namespace HSTL{
    template<typename T1, typename T2>
    class pair{
    public:
        using first_type = T1;
        using second_type = T2;
        first_type first;
        second_type second;

        pair()=default;
        pair(T1 v1, T2 v2): first(v1), second(v2) {}

        bool operator < (const pair<T1, T2> &rhs){
           return this->first < rhs.first || (this->first == rhs.first && this->second < rhs.second);
//            return this->first < rhs.first;
        }

        friend std::ostream &operator << (std::ostream &output, const pair<T1, T2> &data){
            output << data.first << " " << data.second << std::endl;
            return output;
        }
    };
}

#endif //STL_UTILITY_H
