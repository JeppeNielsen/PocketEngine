//
//  Particle.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Particle.h"

void Particle::Reset() {
    mass = 1.0f;
    position = Vector2(0,0);
    positionOld = Vector2(0,0);
    immovable = false;
    lastTriangle = 0;
}

void Particle::SetPosition(Vector2 position) {
    this->position = position;
    positionOld = position;
}