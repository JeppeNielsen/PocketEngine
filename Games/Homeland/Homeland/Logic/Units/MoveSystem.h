//
//  MoveSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 10/08/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Movable.h"
#include "Transform.hpp"

using namespace Pocket;

SYSTEM(MoveSystem, Movable, Transform)
    void Update(float dt);
};
