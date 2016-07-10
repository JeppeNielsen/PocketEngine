//
//  Spring.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Vector2.hpp"
#include "Box.hpp"

using namespace Pocket;

class Particle;

struct Spring {
    public:
        void Reset();
        float length;
        float elasticity;
        float currentLength;
        float tension;
        Particle* particleA;
        Particle* particleB;

    void SetParticles(Particle* a, Particle* b);

    void SetLengthFromParticles();

    void ProjectToAxis(const Vector2& axis, float& min, float& max );

    void CalcBoundingBox(Box& box);

};