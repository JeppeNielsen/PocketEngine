//
//  ComponentHelper.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/05/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <string>
#include "GameWorld.hpp"
#include <map>

using namespace Pocket;

struct ComponentHelper {
    
    struct ComponentInfo {
        std::string name;
        int index;
        std::string category;
    };
    
    using ComponentCollection = std::map<std::string, std::vector<ComponentInfo>>;

    static ComponentCollection GetSortedComponents(GameWorld& world);
    
    static ComponentCollection GetSortedScriptComponents(GameWorld& world, Pocket::ScriptWorld& scriptWorld);
};
