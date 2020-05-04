//
// Created by harpsword on 2020/5/3.
//

#ifndef STL_TYPETRAITS_H
#define STL_TYPETRAITS_H

namespace HSTL{
    struct _true_type { };
    struct _false_type { };

    /*
    ** 萃取传入的T类型的类型特性
    */
    template<class T>
    struct _type_traits
    {
        typedef _false_type		has_trivial_default_constructor;
        typedef _false_type		has_trivial_copy_constructor;
        typedef _false_type		has_trivial_assignment_operator;
        typedef _false_type		has_trivial_destructor;
        typedef _false_type		is_POD_type;
    };

    template<>
    struct _type_traits<bool>
    {
        typedef _true_type		has_trivial_default_constructor;
        typedef _true_type		has_trivial_copy_constructor;
        typedef _true_type		has_trivial_assignment_operator;
        typedef _true_type		has_trivial_destructor;
        typedef _true_type		is_POD_type;
    };
    template<>
    struct _type_traits<char>
    {
        typedef _true_type		has_trivial_default_constructor;
        typedef _true_type		has_trivial_copy_constructor;
        typedef _true_type		has_trivial_assignment_operator;
        typedef _true_type		has_trivial_destructor;
        typedef _true_type		is_POD_type;
    };
    template<>
    struct _type_traits<unsigned char>
    {
        typedef _true_type		has_trivial_default_constructor;
        typedef _true_type		has_trivial_copy_constructor;
        typedef _true_type		has_trivial_assignment_operator;
        typedef _true_type		has_trivial_destructor;
        typedef _true_type		is_POD_type;
    };
    template<>
    struct _type_traits<signed char>
    {
        typedef _true_type		has_trivial_default_constructor;
        typedef _true_type		has_trivial_copy_constructor;
        typedef _true_type		has_trivial_assignment_operator;
        typedef _true_type		has_trivial_destructor;
        typedef _true_type		is_POD_type;
    };









}

#endif //STL_TYPETRAITS_H
