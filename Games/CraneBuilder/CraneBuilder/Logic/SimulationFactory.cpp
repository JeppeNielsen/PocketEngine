//
//  SimulationFactory.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 09/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SimulationFactory.h"
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
#include "Selectable.hpp"

void SimulationFactory::Initialize() {
    World()->CreateOrGetSystem<RenderSystem>();
    
    touchSystem = World()->CreateOrGetSystem<TouchSystem>();
    //creatorSystem = World()->CreateOrGetSystem<CreatorSystem>();
    
    World()->CreateOrGetSystem<DraggableSystem>();
    World()->CreateOrGetSystem<ParticleFollowTransformSystem>();
    simulation = World()->CreateOrGetSystem<SimulationSystem>();
    World()->CreateOrGetSystem<ParticleTransformSystem>();
    World()->CreateOrGetSystem<SpringTransformSystem>();
    World()->CreateOrGetSystem<BeamMeshSystem>();
    World()->CreateOrGetSystem<HydralicMeshSystem>();
    World()->CreateOrGetSystem<ParticleMeshSystem>();
    World()->CreateOrGetSystem<HydralicSystem>();
    World()->CreateOrGetSystem<TerrainMeshSystem>();
}

void SimulationFactory::Setup(Pocket::InputManager* input) {
    
    touchSystem->Input = input;
    
    atlas = World()->CreateObject();
    Texture& texture = atlas->AddComponent<TextureComponent>()->Texture();
    texture.LoadFromPng("images.png");
    atlas->AddComponent<class Atlas>()->Load("images.xml",Vector2(texture.GetWidth(), texture.GetHeight()));
    
    atlas->SetID("atlas");
}

GameObject* SimulationFactory::CreateParticle(Vector2 p) {
    GameObject* p1 = World()->CreateObject();
    p1->AddComponent<Transform>()->Position = p;
    p1->AddComponent<Mesh>();
    p1->AddComponent<Atlas>(atlas);
    p1->AddComponent<TextureComponent>(atlas);
    p1->AddComponent<Material>();//->Shader = &renderSystem->Shaders.Textured;
    p1->GetComponent<Material>()->BlendMode = BlendModeType::Alpha;
    p1->AddComponent<Touchable>();
    //p1->AddComponent<Creator>(creator);
    
    static int counter = 10000;
    counter++;
    
    p1->AddComponent<Selectable>();
    
    p1->AddComponent<Orderable>()->Order = counter;
    p1->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XYPlane;
    Particle* pa = p1->AddComponent<Particle>();
    pa->SetPosition(p);
    return p1;
}

GameObject* SimulationFactory::CreateSpring(float elasticity) {
    GameObject* go = World()->CreateObject();
    go->AddComponent<Transform>();
    go->AddComponent<Mesh>();
    go->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    go->AddComponent<Atlas>(atlas);
    go->AddComponent<TextureComponent>(atlas);
    Spring* spring = go->AddComponent<Spring>();
    spring->elasticity = elasticity;
    
    static int counter = 0;
    counter++;
    go->AddComponent<Orderable>()->Order = counter;
    return go;
}

SimulationSystem* SimulationFactory::Simulation() {
    return simulation;
}