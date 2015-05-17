//
//  FirstPersonMover.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "FirstPersonMover.hpp"

using namespace Pocket;

void FirstPersonMover::Clone(const Pocket::FirstPersonMover &other) {
    MovementSpeed = other.MovementSpeed;
    RotationSpeed = other.RotationSpeed;
    MaxPitch = other.MaxPitch;
}

void FirstPersonMover::Reset() {
    MovementSpeed = 0.2f;
    RotationSpeed = 0.15f;
    MaxPitch = 89.0f;
}

