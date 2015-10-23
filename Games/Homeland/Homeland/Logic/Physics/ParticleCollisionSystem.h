//
//  ParticleCollisionSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 23/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Particle.h"

using namespace Pocket;

class ParticleCollisionSystem : public GameSystem {
public:
    void Initialize();
    void Update(float dt);
};