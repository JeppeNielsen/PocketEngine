//
//  GridSystem.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 12/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Grid.h"
#include "Transform.hpp"

struct GridSystem: public GameSystem<Grid, Transform> {

    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);

};