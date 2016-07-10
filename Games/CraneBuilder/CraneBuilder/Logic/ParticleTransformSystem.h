//
//  ParticleTransformSystem.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/3/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Particle.h"

using namespace Pocket;

class ParticleTransformSystem : public GameSystem<Transform, Particle>  {
public:
    void Update(float dt);
};