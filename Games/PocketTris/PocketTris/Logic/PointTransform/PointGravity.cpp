//
//  PointGravity.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PointGravity.hpp"

void PointGravity::Reset() {
    Gravity = 0;
    Timer = 0;
    movedDownThisFrame = false;
    lastPosition = 0;
}

