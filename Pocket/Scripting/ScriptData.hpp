//
//  ScriptData.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 15/05/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include <vector>
#include <string>
#include <functional>

namespace Pocket {
    class ScriptData {
    public:
        bool Parse(const std::vector<std::string>& cppFiles, const std::vector<std::string>& includePaths, const std::function<bool(const std::string&)>& predicate);
        
        struct System {
            std::string name;
            std::vector<std::string> components;
        };
        
        struct Component {
            struct Field {
                std::string name;
                std::string type;
            };
        
            std::string name;
            using Fields = std::vector<Field>;
            Fields fields;
        };
        
        using Systems = std::vector<System>;
        using Components = std::vector<Component>;
        
        Systems systems;
        Components components;
    
        Component* FindComponent(const std::string& name);
        
        void Clear();
    };
}
