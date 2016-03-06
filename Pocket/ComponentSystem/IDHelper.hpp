//
//  GameComponent.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include <iostream>
#include <typeinfo>

class IDHelper {
private:
    static int componentCounter;
    static int systemCounter;
public:
    const static int NumberOfComponents() {
        return componentCounter;
    }
    
    template<typename Class>
    static std::string GetClassName() {
        std::string functionName = __PRETTY_FUNCTION__;
        const std::string token = "Class = ";
        size_t equal = functionName.find(token) + token.size();
        return functionName.substr(equal, functionName.size() - equal - 1);
    }

    template<typename T>
    static int GetComponentID() {
        static int componentID = componentCounter++;
        return componentID;
    }
    
    const static int NumberOfSystems() {
        return systemCounter;
    }
    
    template<typename T>
    static int GetSystemID() {
        static int systemID = systemCounter++;
        return systemID;
    }
};