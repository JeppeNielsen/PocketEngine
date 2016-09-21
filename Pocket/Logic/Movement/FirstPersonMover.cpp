//
//  FirstPersonMover.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "FirstPersonMover.hpp"

using namespace Pocket;

FirstPersonMover::FirstPersonMover() {
    MovementSpeed = 0.2f;
    RotationSpeed = 0.15f;
    MaxPitch = 89.0f;
    MovementTouchIndex = 0;
    RotationTouchIndex = 1;
}

void FirstPersonMover::SetTouchIndices(int movementTouchIndex, int rotationTouchIndex) {
    MovementTouchIndex = movementTouchIndex;
    RotationTouchIndex = rotationTouchIndex;
}