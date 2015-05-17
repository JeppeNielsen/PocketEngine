//
//  PositionLimiter.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponent.hpp"
#include "Vector3.hpp"

namespace Pocket {
    Component(Limitable)
    public:
        void Reset();
        Vector3 min;
        Vector3 max;
    };
}