//
//  SimulationSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SimulationSystem.h"


void SimulationSystem::AddedToWorld(Pocket::GameWorld &world) {
    particleSystem = world.CreateSystem<ParticleUpdaterSystem>();
    springSystem = world.CreateSystem<SpringSystem>();
    springCollisionSystem = world.CreateSystem<SpringCollisionSystem>();
}

void SimulationSystem::Update(float dt) {
    particleSystem->Simulate(0.02f);
    springSystem->ResetTension();
    for (int i=0; i<10; i++) {
        springSystem->Simulate(0.02f);
        springCollisionSystem->Simulate(0.02f);
    }
    
    //springSystem->CheckForBrokenSprings();
}


