//
//  GameWorld.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"

using namespace Pocket;

void GameWorld::AddComponentType(ComponentId componentId, const std::function<void(ComponentInfo& componentInfo)>& constructor) {
    if (componentId>=components.size()) {
        components.resize(componentId + 1, ComponentInfo{});
    }
    
    if (!components[componentId].container) {
        constructor(components[componentId]);
    }
}