//
//  Velocity.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Vector3.hpp"

using namespace Nano;

Component(PieceVelocity)
public:
    void Reset();

    Vector3 velocity;
    Vector3 gravity;
    Vector3 angularVelocity;

};