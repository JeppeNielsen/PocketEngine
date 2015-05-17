//
//  SheetVelocity.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/24/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Velocity.hpp"

using namespace Pocket;

void Velocity::Reset() {
    velocity = Vector3(0,0,0);
    Friction = 0;
}