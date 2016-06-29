//
//  MetaLibrary.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 03/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

namespace Pocket {
  
class TypeInfo;

namespace Meta {

#define HAS_OPTIONAL_METHOD(methodName, signature) \
template<typename, typename T>          \
struct has_ ## methodName {                 \
    static_assert(                      \
        std::integral_constant<T, false>::value, \
        "Second template parameter needs to be of function type.");\
};\
\
template<typename C, typename Ret, typename... Args>\
struct has_ ## methodName<C, Ret(Args...)> {\
public:\
    template<typename T>\
    static constexpr auto check(T*)\
    -> typename\
        std::is_same<\
            decltype( std::declval<T>().methodName( std::declval<Args>()... ) ),\
            Ret\
        >::type;\
\
    template<typename>\
    static constexpr std::false_type check(...);\
\
    typedef decltype(check<C>(0)) type;\
public:\
    static constexpr bool value = type::value;\
};\
\
struct Has ## methodName ## Function {\
    template <typename... Args>\
    using apply = has_ ## methodName<Args..., signature>;\
};

HAS_OPTIONAL_METHOD(GetType, TypeInfo());

}

}