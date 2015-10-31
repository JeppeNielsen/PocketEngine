//
//  Game.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Engine.hpp"
#include "GameWorld.hpp"
#include "Particle.h"
#include "Spring.h"
#include "SimulationSystem.h"
#include "PhysicsEditor.h"

using namespace Pocket;

class Game : public GameState<Game> {
    public:
        void Initialize();
        void Update(float dt);
        void Render();
    private:
        GameWorld world;
        GameObject* camera;
    
        Particle* CreateParticle(Vector2 p);
        Spring* CreateSpring(Particle* p1, Particle* p2, float elasticity = 1.0f);
    
        Particle* CreateBox(Vector2 center, Vector2 size);
        Particle* CreateChain(Vector2 start, Vector2 end, int links, float elasticity = 1.0f);
        Particle* CreateWheel(Vector2 center, float radius, int points);
    
        void MouseDown(TouchEvent e);
        void ButtonDown(std::string button);
    
        SimulationSystem* simulation;
    
};