//
//  SheetVelocity.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/24/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Vector3.hpp"

namespace Pocket {
    Component(Velocity)
    public:
        void Reset();
        Vector3 velocity;
        float Friction;
        float MinimumSpeedBeforeStop;
    };
}