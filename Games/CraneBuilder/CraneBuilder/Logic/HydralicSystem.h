//
//  HydralicSystem.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 02/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Spring.h"
#include "Hydralic.h"
using namespace Pocket;

SYSTEM(HydralicSystem, Spring, Hydralic)
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
};