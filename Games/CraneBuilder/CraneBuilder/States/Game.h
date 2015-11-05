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
#include "CreatorSystem.h"
#include "Hydralic.h"

using namespace Pocket;

class Game : public GameState<Game> {
    public:
        void Initialize();
        void Update(float dt);
        void Render();
    private:
        GameWorld world;
        GameObject* camera;
        GameObject* atlas;
        RenderSystem* renderSystem;
        GameObject* creator;
    
        GameObject* CreateParticle(Vector2 p);
        GameObject* CreateSpring(float elasticity);
    
        /*
        Particle* CreateBox(Vector2 center, Vector2 size);
        Particle* CreateChain(Vector2 start, Vector2 end, int links, float elasticity = 1.0f);
        Particle* CreateWheel(Vector2 center, float radius, int points);
        */
    
        void ButtonDown(std::string button);
        void ButtonUp(std::string button);
    
    
        SimulationSystem* simulation;
        CreatorSystem* creatorSystem;
    
        enum class BuildType {
            Beam,
            Hydralic
        };
    
        BuildType buildType;
    
        Hydralic* currentHydralic;
        bool wireframe;
};