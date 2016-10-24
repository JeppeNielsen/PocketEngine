//
//  SheetMovementSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/24/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Velocity.hpp"

namespace Pocket {
    class VelocitySystem : public GameSystem<Transform, Velocity> {
    public:
        void Update(float dt);
    };
}