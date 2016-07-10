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
#include "Background.h"

void Game::Initialize() {

    creatorSystem = world.CreateSystem<CreatorSystem>();
    creatorSystem->gridSize = 2.0f;
    
    simulationFactory = world.CreateSystem<SimulationFactory>();
    editorFactory = world.CreateSystem<EditorFactory>();
    
    state = world.CreateSystem<ComponentEnablerSystem>();
    state->CurrentState = "editor";
    
    world.CreateSystem<CameraDragSystem>()->Input = &Input;
    
    
    
    
    backgroundSystem = world.CreateSystem<BackgroundSystem>();
    
    simulationFactory->Setup(&Input);
    editorFactory->Setup();
    
    camera = world.CreateObject();
    camera->AddComponent<Camera>()->Viewport = Context().Viewport();
    camera->AddComponent<Transform>()->Position = { 0, 0, 50 };
    camera->GetComponent<Camera>()->FieldOfView = 60;
    camera->AddComponent<CameraDragger>();
    
    
    backgroundSystem->CameraObject = camera;
    
    GameObject* background = world.CreateObject();
    background->AddComponent<Transform>();
    background->AddComponent<Material>();
    background->AddComponent<Mesh>();
    background->AddComponent<Orderable>()->Order = -5000;
    Background* back = background->AddComponent<Background>();
    back->colors[0] = Colour((Colour::Component)255, 223, 166);
    back->colors[3] = back->colors[0];
    back->colors[1] = Colour((Colour::Component)226, 172,102);
    back->colors[2] = back->colors[1];
    
    
    
    
    atlas = world.CreateObject();
    Texture& texture = atlas->AddComponent<TextureComponent>()->Texture();
    texture.LoadFromPng("images.png");
    atlas->AddComponent<class Atlas>()->Load("images.xml",Vector2(texture.GetWidth(), texture.GetHeight()));
    
    buildType = BuildType::Beam;
    
    creator = world.CreateObject();
    creator->AddComponent<Creator>();
    
    currentHydralic = 0;
    
    creatorSystem->CreateParticle = [this] () {
        GameObject* go = simulationFactory->CreateParticle(0);
        go->AddComponent<Creator>(creator);
        go->AddComponent<ComponentEnabler>()->AddComponent<Creator>("editor");
        go->GetComponent<ComponentEnabler>()->AddComponent<Draggable>("simulating");
        
        return go;
    };

    creatorSystem->CreateSpring = [this] () {
        GameObject* s = simulationFactory->CreateSpring(70.0f);
        if (buildType == BuildType::Beam) {
            s->AddComponent<Beam>();
        } else if (buildType == BuildType::Hydralic) {
            currentHydralic = s->AddComponent<Hydralic>();
        }
        s->AddComponent<Creator>(creator);
        s->AddComponent<ComponentEnabler>()->AddComponent<Creator>("editor");
        s->GetComponent<ComponentEnabler>()->AddComponent<Draggable>("simulating");
        
        return s;
    };
    
    creatorSystem->Continuous = [this] () {
        return buildType == BuildType::Beam;
    };
    
    LoadLevel("Level.txt");
    
    Input.ButtonDown .Bind(this, &Game::ButtonDown);
    Input.ButtonUp .Bind(this, &Game::ButtonUp);
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
        state->CurrentState = simulationFactory->Simulation()->Running() ? "simulation" : "editor";
    }
    
    if (button == "b") {
        Plane plane({0,0,1},0);
        Ray ray = camera->GetComponent<Camera>()->GetRay(
                camera->GetComponent<Transform>(),
                Input.GetTouchPosition(0));

        float dist;
        if (plane.IntersectsRay(ray, &dist)) {
            Vector3 position = ray.GetPosition(dist);
            CreateBox(position);
        }
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

void Game::CreateBox(Pocket::Vector3 position) {
    Vector2 size = 5.0f;
    
    GameObject* p1 = simulationFactory->CreateParticle(position + Vector2(-size.x, -size.y));
    GameObject* p2 = simulationFactory->CreateParticle(position + Vector2(-size.x, size.y));
    GameObject* p3 = simulationFactory->CreateParticle(position + Vector2(size.x, size.y));
    GameObject* p4 = simulationFactory->CreateParticle(position + Vector2(size.x, -size.y));
    
    CreateSpring(p1, p2);
    CreateSpring(p2, p3);
    CreateSpring(p3, p4);
    CreateSpring(p4, p1);
    CreateSpring(p1, p3);
    
}

void Game::CreateSpring(Pocket::GameObject *p1, Pocket::GameObject *p2) {
    GameObject* s1 = simulationFactory->CreateSpring(50.0f);
    s1->GetComponent<Spring>()->SetParticles(p1->GetComponent<Particle>(), p2->GetComponent<Particle>());
    s1->AddComponent<Beam>();
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
    //world.CreateObjectFromJson(file, [this] (GameObject* o) {
      //  if (o->GetComponent<Particle>()) {
        //    o->AddComponent<Creator>(creator);
       // }
    //});
}