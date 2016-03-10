//
//  SheetVelocity.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/24/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Vector3.hpp"

namespace Pocket {
    struct Velocity {
        Vector3 velocity;
        float Friction;
        float MinimumSpeedBeforeStop;
    };
}