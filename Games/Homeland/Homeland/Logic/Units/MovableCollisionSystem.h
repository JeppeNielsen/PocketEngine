//
//  MovableCollisionSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 12/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"
#include "Movable.h"
#include "Mappable.h"

using namespace Pocket;

SYSTEM(MovableCollisionSystem, Mappable, Transform, Movable)
public:
    void Update(float dt);
    bool IsPositionValid(Vector3 position, Map* map, Vector3& collisionNormal);
};
