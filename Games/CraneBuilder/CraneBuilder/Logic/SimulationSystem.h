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
#include <map>

using namespace Pocket;

class SimulationSystem : public GameConcept {
public:
    SimulationSystem();
    
    void Initialize();
    void Update(float dt);
    
    Property<bool> Running;

private:
    ParticleUpdaterSystem* particleSystem;
    SpringSystem* springSystem;
    
    void RunningChanged();
    
    typedef std::map<Particle*, Vector2> StoredPositions;
    StoredPositions storedPositions;
    
    void Store();
    void Restore();
public:
    SpringCollisionSystem* springCollisionSystem;

};