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

struct ComponentEnablerSystem: public GameSystem<ComponentEnabler> {
public:
    ComponentEnablerSystem();
    Property<std::string> CurrentState;

    void Initialize();
    void ObjectAdded(GameObject* object);
private:
    void StateChanged();
    void UpdateObject(GameObject* object);
};