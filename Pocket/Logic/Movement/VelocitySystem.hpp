//
//  SheetMovementSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/24/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Transform.hpp"
#include "Velocity.hpp"

namespace Pocket {
    class VelocitySystem : public GameSystem {
    public:
        void Initialize();
        void Update(float dt);
    };
}