//
//  SimulationSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SimulationSystem.h"
#include "Transform.hpp"

SimulationSystem::SimulationSystem() { Running = false; Running.Changed.Bind(this, &SimulationSystem::RunningChanged); }

void SimulationSystem::Initialize() {
    particleSystem = world->CreateSystem<ParticleUpdaterSystem>();
    springSystem = world->CreateSystem<SpringSystem>();
    springCollisionSystem = world->CreateSystem<SpringCollisionSystem>();
}

void SimulationSystem::Update(float dt) {
    if (!Running) return;

    particleSystem->Simulate(0.02f);
    springSystem->ResetTension();
    for (int i=0; i<10; i++) {
        springSystem->Simulate(0.02f);
        springCollisionSystem->Simulate(0.02f);
    }
    
    //springSystem->CheckForBrokenSprings();
}

void SimulationSystem::RunningChanged() {
    if (!Running()) {
        Restore();
    } else {
        Store();
    }
}


void SimulationSystem::Store() {
    storedPositions.clear();
    for(GameObject* go : particleSystem->Objects()) {
        storedPositions[go->GetComponent<Particle>()] = go->GetComponent<Particle>()->position;
        //std::cout<<"stored position: " <<storedPositions[go->GetComponent<Particle>()]<<std::endl;
    }
}

void SimulationSystem::Restore() {
    for(GameObject* go : particleSystem->Objects()) {
        Vector2 position = storedPositions[go->GetComponent<Particle>()];
        go->GetComponent<Particle>()->SetPosition(position);
        go->GetComponent<Transform>()->Position = position;
        //std::cout<<"restored position: " <<storedPositions[go->GetComponent<Particle>()]<<std::endl;
    }
    
    for(GameObject* go : springSystem->Objects()) {
        Spring* spring = go->GetComponent<Spring>();
        spring->tension = 0;
        spring->currentLength = spring->length;
    }
}

