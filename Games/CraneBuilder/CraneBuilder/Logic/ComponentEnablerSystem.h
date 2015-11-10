//
//  ComponentEnablerSystem.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 10/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "ComponentEnabler.h"

SYSTEM(ComponentEnablerSystem, ComponentEnabler)
public:
    ComponentEnablerSystem();
    Property<ComponentEnablerSystem*, std::string> CurrentState;

    void AddedToWorld(GameWorld& world);
    void ObjectAdded(GameObject* object);
private:
    void StateChanged(ComponentEnablerSystem* system);
    void UpdateObject(GameObject* object);
};