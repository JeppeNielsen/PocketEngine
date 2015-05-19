//
//  PointGravity.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Point.hpp"
using namespace Pocket;

Component(PointGravity)
public:
    void Reset();

    float Gravity;
    float Timer;
    bool movedDownThisFrame;
    Point lastPosition;

};