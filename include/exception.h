//
// Created by harpsword on 2020/4/22.
//

#ifndef STL_EXCEPTION_H
#define STL_EXCEPTION_H

namespace HSTL{
    class exception {
    public:
        exception() {}
        ~exception() {}
        virtual char *what(){ }
    };

    class out_of_range: exception{
    public:
        out_of_range(char *c): msg(c) {}
        ~out_of_range(){}

        virtual char *what(){
            return msg;
        }

    private:
        char *msg;
    };
}

#endif //STL_EXCEPTION_H
