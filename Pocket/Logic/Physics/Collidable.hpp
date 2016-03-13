//
//  Collidable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 29/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObject.hpp"
#include "Property.hpp"
#include "Vector3.hpp"

namespace Pocket {

    class Collidable;
    
    struct CollisionInfo {
        GameObject* object;
        Collidable* collidable;
        
        GameObject* collidedObject;
        Collidable* collidableObject;
        
        Vector3 worldPosition;
        Vector3 worldNormal;
    };

    class  Collidable {
    public:
        Collidable();
        Property<bool> IsColliding;
        Event<CollisionInfo> Enter;
        Event<CollisionInfo> Exit;
    
        int collisionCounts;
    };
}
