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
#include "SimulationFactory.h"
#include "EditorFactory.h"

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
        GameObject* creator;
        SimulationFactory* simulationFactory;
        EditorFactory* editorFactory;
    
        void ButtonDown(std::string button);
        void ButtonUp(std::string button);
        void LoadLevel(std::string filename);
    
        CreatorSystem* creatorSystem;
    
        enum class BuildType {
            Beam,
            Hydralic
        };
    
        BuildType buildType;
        Hydralic* currentHydralic;
        bool wireframe;
};