//
//  MovableAlignmentSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 11/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Movable.h"
#include "Transform.hpp"

using namespace Pocket;

SYSTEM(MovableAlignmentSystem, Movable, Transform)
    public:
        void Update(float dt);
};