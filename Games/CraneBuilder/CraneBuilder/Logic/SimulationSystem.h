//
//  SimulationSystem.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "ParticleUpdaterSystem.h"
#include "SpringSystem.h"
#include "SpringCollisionSystem.h"

using namespace Pocket;

class SimulationSystem : public GameSystem {
public:
    void AddedToWorld(GameWorld& world);
    void Update(float dt);

private:
    ParticleUpdaterSystem* particleSystem;
    SpringSystem* springSystem;
public:
    SpringCollisionSystem* springCollisionSystem;

};