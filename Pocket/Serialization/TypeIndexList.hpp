//
//  TypeIndexList.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 26/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <map>
#include <string>
#include "Property.hpp"

namespace Pocket {

class IFieldInfo;

template<typename T>
class FieldInfo;

class TypeInfo;

  struct TypeIndexList {

    template<typename T>
    void Add() {
       AddInternal<T>();
       AddInternal<std::vector<T>>();
       AddInternal<Property<T>>();
    }
    
    void AddToTypeInfo(TypeInfo* info, int index, IFieldInfo* fieldInfo) {
        auto it = types.find(index);
        if (it!=types.end()) {
            return it->second->AddToTypeInfo(info, fieldInfo);
        }
    }
    
    std::map<int, std::string> GetTypeNames() {
        std::map<int, std::string> names;
        for(auto t : types) {
            names[t.first] = t.second->GetName();
        }
        return names;
    }
    
    template<typename T>
    static int Index() {
        static int indexer = indexCounter++;
        return indexer;
    }

private:
    template<typename Class>
    static std::string GetClassName() {
        std::string functionName = __PRETTY_FUNCTION__;
        const std::string token = "Class = ";
        size_t equal = functionName.find(token) + token.size();
        return functionName.substr(equal, functionName.size() - equal - 1);
    }
    
    struct ITypeIndex {
        virtual ~ITypeIndex() {}
        virtual std::string GetName() = 0;
        virtual void AddToTypeInfo(TypeInfo* typeInfo, IFieldInfo* fieldInfo) = 0;
    };

    template<typename T>
    struct TypeIndex : public ITypeIndex {
        std::string GetName() override {
            return GetClassName<T>();
        };
        void AddToTypeInfo(Pocket::TypeInfo *typeInfo, Pocket::IFieldInfo* fieldInfo) override;
    };

    static int indexCounter;
    
    using Types = std::map<int, ITypeIndex*>;
    Types types;

    template<typename T>
    void AddInternal() {
         types[Index<T>()] = new TypeIndex<T>();
    }
};



}