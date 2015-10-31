//
//  ParticleFollowTransformSystem.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/4/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Particle.h"

using namespace Pocket;

class ParticleFollowTransformSystem : public GameSystem {
public:
    void Initialize();
    void Update(float dt);

};