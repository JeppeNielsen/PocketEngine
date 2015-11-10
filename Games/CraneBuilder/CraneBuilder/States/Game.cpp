//
//  Game.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Game.h"
#include "RenderSystem.hpp"
#include "SimulationSystem.h"
#include "ParticleTransformSystem.h"
#include "ParticleFollowTransformSystem.h"
#include "TouchSystem.hpp"
#include "DraggableSystem.hpp"
#include "SpringTransformSystem.h"
#include "Orderable.hpp"
#include "BeamMeshSystem.h"
#include "HydralicMeshSystem.h"
#include "ParticleMeshSystem.h"
#include "HydralicSystem.h"
#include "TerrainMeshSystem.h"
#include "EditorFactory.h"
#include <fstream>
#include "CameraDragSystem.h"

void Game::Initialize() {

    simulationFactory = world.CreateFactory<SimulationFactory>();
    editorFactory = world.CreateFactory<EditorFactory>();
    
    world.CreateSystem<CameraDragSystem>()->Input = &Input;
    
    creatorSystem = world.CreateSystem<CreatorSystem>();
    
    simulationFactory->Setup(&Input);
    editorFactory->Setup();
    
    camera = world.CreateObject();
    camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
    camera->AddComponent<Transform>()->Position = { 0, 0, 50 };
    camera->GetComponent<Camera>()->FieldOfView = 60;
    camera->AddComponent<CameraDragger>();
    
    atlas = world.CreateObject();
    Texture& texture = atlas->AddComponent<TextureComponent>()->Texture();
    texture.LoadFromPng("images.png");
    atlas->AddComponent<class Atlas>()->Load("images.xml",Vector2(texture.GetWidth(), texture.GetHeight()));
    
    buildType = BuildType::Beam;
    
    creator = world.CreateObject();
    creator->AddComponent<Creator>();
    
    currentHydralic = 0;
    
    creatorSystem->CreateParticle = [this] () -> GameObject* {
        return simulationFactory->CreateParticle(0);
    };

    creatorSystem->CreateSpring = [this] () -> GameObject* {
        GameObject* s = simulationFactory->CreateSpring(70.0f);
        if (buildType == BuildType::Beam) {
            s->AddComponent<Beam>();
        } else if (buildType == BuildType::Hydralic) {
            currentHydralic = s->AddComponent<Hydralic>();
        }
        return s;
    };
    
    creatorSystem->Continuous = [this] () -> bool {
        return buildType == BuildType::Beam;
    };
    
    LoadLevel("Level.txt");
    
    Input.ButtonDown += event_handler(this, &Game::ButtonDown);
    Input.ButtonUp += event_handler(this, &Game::ButtonUp);
    wireframe = false;
}

void Game::ButtonUp(std::string button) {
    if (button == "" && currentHydralic) {
        currentHydralic->speed = 0.0f;
    }
    if (button == "" && currentHydralic) {
        currentHydralic->speed = 0.0f;
    }
}

void Game::ButtonDown(std::string button) {

    if (button == " ") {
        creatorSystem->Cancel();
        simulationFactory->Simulation()->Running = !simulationFactory->Simulation()->Running();
    }

    if (button == "1") buildType = BuildType::Beam;
    if (button == "2") buildType = BuildType::Hydralic;
    
    if (button == "" && currentHydralic) {
        currentHydralic->speed = 3.0f;
    }
    
    if (button == "" && currentHydralic) {
        currentHydralic->speed = -3.0f;
    }
    
    if (button == "w") wireframe =!wireframe;
}


void Game::Update(float dt) {
    world.Update(dt);
}

void Game::Render() {
    if (wireframe) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        } else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
    glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world.Render();
}

void Game::LoadLevel(std::string filename) {
    std::ifstream file;
    file.open(filename);
    GameObject* level = world.CreateObjectFromJson(file, [this] (GameObject* o) {
        if (o->GetComponent<Particle>()) {
            o->AddComponent<Creator>(creator);
        }
    });
}