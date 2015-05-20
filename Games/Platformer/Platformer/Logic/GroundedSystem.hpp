//
//  GroundedSystem.h
//  Platformer
//
//  Created by Jeppe Nielsen on 29/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Groundable.hpp"
#include "Collidable.hpp"
#include <vector>

using namespace Nano;

SYSTEM(GroundedSystem, Groundable, Collidable)
public:
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);

    void Update(float dt);

private:

    struct GroundedObject {
        GameObject* object;
    };

    typedef std::vector<GroundedObject> GroundedObjects;

    void IsGroundedChanged(Collidable* collidable, GameObject* object);

};
