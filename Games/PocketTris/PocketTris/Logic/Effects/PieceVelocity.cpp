//
//  Velocity.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PieceVelocity.hpp"

void PieceVelocity::Reset() {
    velocity = Vector3(0, 0, 0);
    gravity = Vector3(0, 0, 0);
    angularVelocity = Vector3(0, 0, 0);
}