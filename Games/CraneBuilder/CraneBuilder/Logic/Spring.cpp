//
//  Spring.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Spring.h"
#include <cmath>
#include "Particle.h"

Spring::Spring() {
    length = 0.0f;
    particleA = 0;
    particleB = 0;
    elasticity = 1.0f;
}

void Spring::SetParticles(Particle *a, Particle *b) {
    particleA = a;
    particleB = b;
    SetLengthFromParticles();
}

void Spring::SetLengthFromParticles() {
    if (!particleA) return;
    if (!particleB) return;
    length = (particleA->position-particleB->position).Length();
    currentLength = length;
}

#define MIN(X,Y) ((X<Y) ? (X) : (Y))
#define MAX(X,Y) ((X>Y) ? (X) : (Y))

void Spring::ProjectToAxis(const Vector2& axis, float& min, float& max ) {
    float dot = axis.Dot(particleA->position);
    min = max = dot;
    dot = axis.Dot(particleB->position);
    min = MIN( dot, min );
    max = MAX( dot, max );
}

void Spring::CalcBoundingBox(Pocket::Box &box) {
    Vector2 min = particleA->position.Min(particleB->position);
    Vector2 max = particleA->position.Max(particleB->position);
    box.left = min.x;
    box.bottom = min.y;
    box.right = max.x;
    box.top = max.y;
}