//
//  SimulationFactory.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 09/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "InputManager.hpp"
#include "TouchSystem.hpp"
#include "SimulationSystem.h"

using namespace Pocket;

class SimulationFactory : public GameFactory {
public:
    void Initialize();
    void Setup(InputManager* input);

    GameObject* CreateParticle(Vector2 position);
    GameObject* CreateSpring(float elasticity);
    
    SimulationSystem* Simulation();
private:
    SimulationSystem* simulation;
    TouchSystem* touchSystem;
    GameObject* atlas;
};