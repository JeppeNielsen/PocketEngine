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
        
        Velocity() : velocity(0), Friction(0), MinimumSpeedBeforeStop(0) {}
    
        Vector3 velocity;
        float Friction;
        float MinimumSpeedBeforeStop;
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(velocity)
        TYPE_FIELDS_END
    };
}