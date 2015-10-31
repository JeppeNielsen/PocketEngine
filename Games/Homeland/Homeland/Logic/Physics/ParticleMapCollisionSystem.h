//
//  ParticleMapCollisionSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 23/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Particle.h"
#include "Mappable.h"
#include "Groundable.h"

SYSTEM(ParticleMapCollisionSystem, Groundable, Particle, Mappable)
    public:
        void Update(float dt);
};