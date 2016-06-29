//
//  GameIDHelper.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 06/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include <bitset>
#include <array>
#include <functional>
#include "Container.hpp"
#include "Bitset.hpp"

namespace Pocket {
    
    using ComponentID = int;
    
    using SystemID = int;
    
    class GameIDHelper {
    private:
        static ComponentID componentIDCounter;
        static SystemID systemIDCounter;
        
    public:
    
        template<typename T>
        static ComponentID GetComponentID() {
            static ComponentID id = componentIDCounter++;
            return id;
        }
        
        template<typename T>
        static SystemID GetSystemID() {
            static SystemID id = systemIDCounter++;
            return id;
        }
        
        template<typename Class>
        static std::string GetClassName() {
            std::string functionName = __PRETTY_FUNCTION__;
            const std::string token = "Class = ";
            size_t equal = functionName.find(token) + token.size();
            return functionName.substr(equal, functionName.size() - equal - 1);
        }
    };
}