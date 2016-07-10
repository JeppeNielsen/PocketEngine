//
//  EditorFactory.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 09/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "EditorFactory.h"
#include "TextureComponent.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Orderable.hpp"
#include "Touchable.hpp"
#include "TouchableCanceller.hpp"
#include "Terrain.h"
#include "Atlas.hpp"
#include "Particle.h"
#include "Draggable.hpp"
#include "ComponentEnabler.h"
#include "TerrainEditableVertices.h"
#include "TransformHierarchy.hpp"

void EditorFactory::Initialize() {
    world->CreateSystem<TransformHierarchy>();
}

void EditorFactory::Setup() {

    terrainTexture = world->CreateObject();
    terrainTexture->AddComponent<TextureComponent>()->Texture().LoadFromPng("Terrain.png");
    //terrainTexture->SetID("terrain");

    dirtTexture = world->CreateObject();
    dirtTexture->AddComponent<TextureComponent>()->Texture().LoadFromPng("Dirt.png");
    //dirtTexture->SetID("dirt");
}

GameObject* EditorFactory::CreateTerrain(Pocket::Vector3 position) {

    GameObject* t = world->CreateObject();
    t->AddComponent<Transform>()->Position = position;
    t->AddComponent<Mesh>();
    t->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    t->AddComponent<TextureComponent>(terrainTexture);
    t->AddComponent<Orderable>()->Order = -50;
    t->AddComponent<Touchable>();
    t->AddComponent<TouchableCanceller>()->MovementToCancel = 10;
    t->GetComponent<TouchableCanceller>()->trackTouchPosition = true;
    t->AddComponent<TerrainEditableVertices>();
    t->AddComponent<ComponentEnabler>()->AddComponent<Touchable>("terrain");
    t->GetComponent<ComponentEnabler>()->AddComponent<TerrainEditableVertices>("terrain");
    
    Terrain* terrain = t->AddComponent<Terrain>();
    
    //terrain->vertices = {
    //    {-50,0},{-50,-10},{-20,-10},{-20,-50},{-50,-50}, {-50,-60},{50,-60},{50,-50},{20,-50},{20,-10},{50,-10},{50,0},
    //};
    terrain->vertices = { {-10,-10} ,{10,-10},{10,10},{-10,10}};
    
    Terrain::Layer grass;
    grass.outerDepth = -3;
    grass.innerDepth = 0;
    grass.textureScale  = 0.06f;
    grass.outerV = 8.0f / 256.0f;
    grass.innerV = 67.0f / 256.0f;
    grass.isBevel = false;
    grass.useDirection = true;
    grass.direction = {0,1};
    grass.directionAmount = 0;
    
    terrain->layers.push_back(grass);
    
    Terrain::Layer shadows;
    shadows.outerDepth = -1;
    shadows.innerDepth = 0.25f;
    shadows.textureScale  = 0.06f;
    shadows.outerV = 69.0f / 256.0f;
    shadows.innerV = 129.0f / 256.0f;
    shadows.isBevel = false;
    shadows.useDirection = true;
    shadows.direction = {0,-1};
    shadows.directionAmount = 0.4f;
    shadows.color = Colour::White(0.5f);
    
    terrain->layers.push_back(shadows);
    
    Terrain::Layer outline;
    outline.outerDepth = -0.35f;
    outline.innerDepth = 0.35f;
    outline.textureScale  = 0.1f;
    outline.outerV = 69.0f / 256.0f;
    outline.innerV = 118.0f / 256.0f;
    outline.isBevel = false;
    
    terrain->layers.push_back(outline);
    
    Terrain::Layer bevel;
    bevel.outerDepth = 0.3f;
    bevel.innerDepth = 5;
    bevel.textureScale  = 0.1f;
    bevel.outerV = 2.0f / 256.0f;
    bevel.innerV = bevel.outerV;
    bevel.isBevel = true;
    
    terrain->layers.push_back(bevel);
    
    GameObject* center = world->CreateObject();
    center->Parent() = t;
    center->AddComponent<Transform>();
    center->AddComponent<Mesh>();
    center->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    center->AddComponent<TextureComponent>(dirtTexture);
    center->AddComponent<Orderable>()->Order = -100;
    center->AddComponent<Touchable>(t);
    center->AddComponent<ComponentEnabler>()->AddComponent<Touchable>("terrain");
    return t;
}

GameObject* EditorFactory::CreateStaticParticle(Pocket::Vector3 position) {
    GameObject* p1 = world->CreateObject();
    p1->AddComponent<Transform>()->Position = position;
    p1->AddComponent<Mesh>();
    p1->AddComponent<Atlas>(atlas);
    p1->AddComponent<TextureComponent>(atlas);
    p1->AddComponent<Material>();//->Shader = &renderSystem->Shaders.Textured;
    p1->GetComponent<Material>()->BlendMode = BlendModeType::Alpha;
    p1->AddComponent<Touchable>();
    
    static int counter = 10000;
    counter++;
    
    p1->AddComponent<Orderable>()->Order = counter;
    p1->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XYPlane;
    Particle* pa = p1->AddComponent<Particle>();
    pa->immovable = true;
    pa->SetPosition(position);
    return p1;
}
