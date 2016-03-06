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

template<class F, class...Ts, std::size_t...Is>
constexpr void for_each_in_tuple(std::tuple<Ts...> & tuple, F&& func, std::index_sequence<Is...>){
    using expander = int[];
    (void)expander { 0, ((void)func(std::get<Is>(tuple)), 0)... };
}

template<class F, class...Ts>
constexpr void for_each_in_tuple_non_const(std::tuple<Ts...> & tuple, F&& func){
    for_each_in_tuple(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
}


namespace static_if_detail {

template<typename Param, bool Cond>
struct statement {
    template<typename F>
    void then(Param param, const F& f){
        f(param);
    }
};

template<typename Param>
struct statement<Param, false> {
    template<typename F>
    void then(Param param, const F&){}
};

} //end of namespace static_if_detail

template<bool Cond, typename Param, typename F>
static_if_detail::statement<Param, Cond> static_if(Param param, F const& f){
    static_if_detail::statement<Param, Cond> if_;
    if_.then(param, f);
    return if_;
}


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