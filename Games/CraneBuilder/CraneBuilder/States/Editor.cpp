//
//  Editor.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 07/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Editor.h"
#include "RenderSystem.hpp"
#include "TerrainMeshSystem.h"
#include "CameraDragSystem.h"
#include "TerrainVertexEditor.h"

void Editor::Initialize() {
    world.CreateSystem<RenderSystem>();
    world.CreateSystem<TerrainMeshSystem>();
    world.CreateSystem<CameraDragSystem>()->Input = &Input;
    world.CreateSystem<TerrainVertexEditor>()->SetInput(&Input);
    
    GameObject* camera = world.CreateObject();
    camera->AddComponent<Transform>()->Position = {0,0,40};
    camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
    camera->AddComponent<CameraDragger>();
    
    GameObject* t = world.CreateObject();
    t->AddComponent<Transform>();
    t->AddComponent<Mesh>();
    t->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    t->AddComponent<TextureComponent>()->Texture().LoadFromPng("Terrain.png");
    t->AddComponent<Orderable>()->Order = -50;
    t->AddComponent<TerrainEditableVertices>();
    t->AddComponent<Touchable>();
    Terrain* terrain = t->AddComponent<Terrain>();
    
    terrain->vertices = {
        {-50,0},{-50,-10},{-20,-10},{-20,-50},{-50,-50}, {-50,-60},{50,-60},{50,-50},{20,-50},{20,-10},{50,-10},{50,0},
    };
    
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
    
    GameObject* center = world.CreateObject();
    center->Parent = t;
    center->AddComponent<Transform>();
    center->AddComponent<Mesh>();
    center->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    center->AddComponent<TextureComponent>()->Texture().LoadFromPng("Dirt.png");
    center->AddComponent<Orderable>()->Order = -100;
    center->AddComponent<Touchable>(t);
    
    terrain->centerMesh = center->GetComponent<Mesh>();
}

void Editor::Update(float dt) {
    world.Update(dt);
}

void Editor::Render() {
    world.Render();
}