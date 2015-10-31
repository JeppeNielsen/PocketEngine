//
//  ParticleSystem.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Particle.h"

using namespace Pocket;

class ParticleUpdaterSystem : public GameSystem {
public:
    void Initialize();
    void Simulate(float dt);

};