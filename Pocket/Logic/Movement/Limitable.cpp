//
//  PositionLimiter.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Limitable.hpp"

using namespace Pocket;

Limitable::Limitable() {
    min = Vector3(0,0,0);
    max = Vector3(0,0,0);
    Size = 0;
    View = 0;
}

