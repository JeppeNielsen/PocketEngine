//
//  GameSettings.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "metaLib.hpp"
#include "GameSystem.hpp"
#include <vector>
#include <bitset>
#include <array>
#include "Container.hpp"
#include "TypeInfo.hpp"

template<typename...T>
struct GameSettings {

    using SystemsInList = meta::list<T...>;
    
    using AllSystems = meta::concat<SystemsInList, meta::as_list<decltype(meta::FindSystems<SystemsInList>{}.Iterate())>>;

    using Systems = meta::unique<AllSystems>;

    constexpr static size_t NumberOfSystems = Systems{}.size();
    
    using AllComponentsInSystems = meta::as_list<decltype(meta::FindComponents<Systems>{}.Iterate())>;
    using UniqueComponents = meta::unique<AllComponentsInSystems>;
    
    template <typename... Ts>
    using TupleOfContainers = std::tuple<Container<Ts>...>;
    
    template <typename... Ts>
    using TupleOfPointers = std::tuple<Ts*...>;
    
    using SystemsTuple = meta::mp_rename<Systems, std::tuple>;
    using InitializeSystems = meta::mp_rename<meta::filter<Systems, meta::HasInitializeFunction>, TupleOfPointers>;
    using UpdateSystems = meta::mp_rename<meta::filter<Systems, meta::HasUpdateFunction>, TupleOfPointers>;
    using RenderSystems = meta::mp_rename<meta::filter<Systems, meta::HasRenderFunction>, TupleOfPointers>;
    using ObjectAddedSystems = meta::mp_rename<meta::filter<Systems, meta::HasObjectAddedFunction>, TupleOfPointers>;
    using ObjectRemovedSystems = meta::mp_rename<meta::filter<Systems, meta::HasObjectAddedFunction>, TupleOfPointers>;
    
    using AllComponents = meta::mp_rename<UniqueComponents, TupleOfContainers>;
    
    using Bitset = std::bitset<UniqueComponents{}.size()>;
    using SystemBitsets = std::array<Bitset, NumberOfSystems>;
    
    using ComponentSystems = meta::as_list<decltype(meta::FindComponentSystems<Systems, UniqueComponents>{}.Iterate())>;
    using ComponentSystemsTuple = meta::mp_rename<ComponentSystems, std::tuple>;
    
    using SerializableComponents = meta::mp_rename<meta::filter<UniqueComponents, meta::HasGetTypeFunction>, TupleOfPointers>;
    
    using ComponentNames = std::array<std::string, UniqueComponents{}.size()>;
    
    template<typename System>
    constexpr static size_t GetSystemID() {
        static_assert(meta::find_index<Systems, System> {} != meta::npos{}, "System is not valid");
        return meta::find_index<Systems, System> {};
    }
    
    template<typename Component>
    constexpr static size_t GetComponentID() {
        static_assert(meta::find_index<UniqueComponents, Component> {} != meta::npos{}, "Component is not valid");
        return meta::find_index<UniqueComponents, Component> {};
    }
    
    template<typename Class>
    static std::string GetClassName() {
        std::string functionName = __PRETTY_FUNCTION__;
        const std::string token = "Class = ";
        size_t equal = functionName.find(token) + token.size();
        return functionName.substr(equal, functionName.size() - equal - 1);
    }
    
    static ComponentNames GetComponentNames() {
        using Components = meta::mp_rename<UniqueComponents, TupleOfPointers>;
        Components components;
        ComponentNames componentNames;
        meta::for_each_in_tuple(components, [&componentNames] (auto component) {
            using ComponentType = std::remove_pointer_t<decltype(component)>;
            componentNames[GetComponentID<ComponentType>()] = GetClassName<ComponentType>();;
        });
        return componentNames;
    }
};