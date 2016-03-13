//
//  PositionLimiter.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Vector3.hpp"
#include "Sizeable.hpp"

namespace Pocket {
    class Limitable {
    public:
        Limitable();
        Vector3 min;
        Vector3 max;
    
        Sizeable* Size;
        Sizeable* View;
    };
}