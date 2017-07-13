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
    
    using ComponentId = int;
    
    using SystemId = int;
    
    class GameIdHelper {
    private:
        static ComponentId componentIDCounter;
        static SystemId systemIDCounter;
        
        template<typename Class>
        static std::string GetClassNameInternal() {
#ifdef WIN32
			const std::string functionName = __FUNCTION__;
#else
            const std::string functionName = __PRETTY_FUNCTION__;
#endif
            const std::string token = "Class = ";
            size_t equal = functionName.find(token) + token.size();
            return functionName.substr(equal, functionName.size() - equal - 1);
        }
        
    public:
    
        template<typename T>
        static ComponentId GetComponentID() {
            static ComponentId id = componentIDCounter++;
            return id;
        }
        
        template<typename T>
        static SystemId GetSystemID() {
            static SystemId id = systemIDCounter++;
            return id;
        }
        
        template<typename Class>
        static std::string& GetClassName() {
            static std::string name = GetClassNameInternal<Class>();
            return name;
        }
    };
}
