//
//  metaLib.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "meta.hpp"
#include <tuple>

//struct GameWorld;
struct TypeInfo;

namespace meta {

template<class A, template<class...> class B> struct mp_rename_impl;

template<template<class...> class A, class... T, template<class...> class B>
    struct mp_rename_impl<A<T...>, B>
{
    using type = B<T...>;
};

template<class A, template<class...> class B>
using mp_rename = typename mp_rename_impl<A, B>::type;


template <typename>
struct Iterator;

template <typename... Ts>
struct Iterator<meta::list<Ts...>>
{
    constexpr auto Iterate() const {
        return std::tuple<>{ };
    }
};

template <typename TOther, typename... Ts>
struct Iterator<meta::list<TOther, Ts...>>
{
	constexpr auto Iterate() const {
        return std::tuple_cat(std::tuple<TOther>{}, Iterator<meta::list<Ts...>>{}.Iterate());
    }
};


template <typename>
struct IteratorPointer;

template <typename... Ts>
struct IteratorPointer<meta::list<Ts...>>
{
    constexpr auto Iterate() const {
        return std::tuple<>{ };
    }
};

template <typename TOther, typename... Ts>
struct IteratorPointer<meta::list<TOther, Ts...>>
{
	constexpr auto Iterate() const {
        return std::tuple_cat(std::tuple<TOther*>{}, IteratorPointer<meta::list<Ts...>>{}.Iterate());
    }
};

template <typename>
struct FindComponents;

template <typename... Ts>
struct FindComponents<meta::list<Ts...>>
{
    constexpr auto Iterate() const {
        return std::tuple<>{};
    }
};

template <typename TOther, typename... Ts>
struct FindComponents<meta::list<TOther, Ts...>>
{
	constexpr auto Iterate() const {
	    return std::tuple_cat(FindComponents<meta::list<Ts...>>{}.Iterate(), Iterator<typename TOther::Components>{}.Iterate());
	}
};

//---------------------------------------------


template <typename>
struct IteratorSystems;

template <typename... Ts>
struct IteratorSystems<meta::list<Ts...>>
{
    constexpr auto Iterate() const {
        return std::tuple<>{ };
    }
};

template <typename TOther, typename... Ts>
struct IteratorSystems<meta::list<TOther, Ts...>>
{
	constexpr auto Iterate() const {
        return std::tuple_cat(std::tuple_cat(IteratorSystems<meta::list<Ts...>>{}.Iterate(),std::tuple<TOther>{}), IteratorSystems<typename TOther::Systems>{}.Iterate());
    }
};

template <typename>
struct FindSystems;

template <typename... Ts>
struct FindSystems<meta::list<Ts...>>
{
    constexpr auto Iterate() const {
        return std::tuple<>{};
    }
};

template <typename TOther, typename... Ts>
struct FindSystems<meta::list<TOther, Ts...>>
{
	constexpr auto Iterate() const {
	    return std::tuple_cat(FindSystems<meta::list<Ts...>>{}.Iterate(), IteratorSystems<typename TOther::Systems>{}.Iterate());
	}
};

//---------------------------------------------

template<class F, class...Ts, std::size_t...Is>
constexpr void for_each_in_tuple(const std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...>){
    using expander = int[];
    (void)expander { 0, ((void)func(std::get<Is>(tuple)), 0)... };
}

template<class F, class...Ts>
constexpr void for_each_in_tuple(const std::tuple<Ts...> & tuple, F func){
    for_each_in_tuple(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
}

//---------------------------------------------

template<class F, class...Ts, std::size_t...Is>
constexpr void for_each_in_tuple_non_const(std::tuple<Ts...> & tuple, F&& func, std::index_sequence<Is...>){
    using expander = int[];
    (void)expander { 0, ((void)func(std::get<Is>(tuple)), 0)... };
}

template<class F, class...Ts>
constexpr void for_each_in_tuple_non_const(std::tuple<Ts...> & tuple, F&& func){
    for_each_in_tuple_non_const(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
}

template<typename Component, typename System, typename... Args>
struct has_component {
public:
    template<typename T>
    static constexpr auto check() {
        return std::true_type {};
    }

    typedef typename std::conditional<
    meta::find_index<typename System::Components, Component>{}!=meta::npos{},
    std::true_type, std::false_type>::type type;
};

template<typename Component>
struct ContainsComponent {
    template <typename... Args>
    using apply = has_component<Component, Args...>;
};

template <typename, typename>
struct FindComponentSystems;

template <typename Systems, typename... Ts>
struct FindComponentSystems<Systems, meta::list<Ts...>>
{
    constexpr auto Iterate() const {
        return std::tuple<>{};
    }
};

template <typename Systems, typename TOther, typename... Ts>
struct FindComponentSystems<Systems, meta::list<TOther, Ts...>>
{
    template<typename...T>
    using TupleOfPointers = std::tuple<T*...>;

	constexpr auto Iterate() const {
        using systemsThatHaveComponent = meta::filter<Systems, ContainsComponent<TOther>>;
        using tuple = mp_rename<systemsThatHaveComponent, TupleOfPointers>;
        
	    return std::tuple_cat(std::tuple<tuple>{ }, FindComponentSystems<Systems, meta::list<Ts...>>{}.Iterate());
	}
};


template<typename TFirst, typename...T>
struct ReturnType {
    using t = TFirst;
    using type = ReturnType<TFirst, T...>;
};

template<typename... T>
using ReturnContainedType = typename ReturnType<T...>::t;





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

HAS_OPTIONAL_METHOD(Initialize, void());
HAS_OPTIONAL_METHOD(Update, void(float));
HAS_OPTIONAL_METHOD(Render, void());

HAS_OPTIONAL_METHOD(GetType, TypeInfo());

}