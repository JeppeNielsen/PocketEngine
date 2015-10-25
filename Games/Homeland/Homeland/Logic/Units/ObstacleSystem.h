//
//  ObstacleSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 25/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Obstacle.h"
#include "Transform.hpp"
#include "GameWorld.hpp"
#include "Mappable.h"

SYSTEM(ObstacleSystem, Transform, Obstacle, Mappable)
public:
    void AddedToWorld(GameWorld& world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
    void UpdateMap(Map* map, Transform* transform, const Point& size, int addition);
    Map* map;
    bool isDirty;
};