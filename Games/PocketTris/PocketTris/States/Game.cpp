//
//  Game.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Game.hpp"
#include "RenderSystem.hpp"
#include "PointTransformSystem.hpp"
#include "PointGravitySystem.hpp"
#include "PieceCollisionSystem.hpp"
#include "PieceControllerSystem.hpp"
#include "PieceMeshSystem.hpp"
#include "PieceDestructorSystem.hpp"
#include "PieceSpawnerSystem.hpp"
#include "PieceLineDetectorSystem.hpp"
#include "PieceVelocitySystem.hpp"


void Game::Initialize() {
    
    world.CreateSystem<RenderSystem>();
    
    PieceSpawnerSystem* spawner = world.CreateSystem<PieceSpawnerSystem>();
    world.CreateSystem<PieceControllerSystem>()->SetInputManager(Input);
    world.CreateSystem<PointGravitySystem>();
    world.CreateSystem<PieceCollisionSystem>();
    world.CreateSystem<PointTransformSystem>();
    world.CreateSystem<PieceDestructorSystem>();
    world.CreateSystem<PieceLineDetectorSystem>();
    world.CreateSystem<PieceMeshSystem>();
    world.CreateSystem<PieceVelocitySystem>();
    
    spawner->CreatePieces();
    
    camera = world.CreateObject();
    camera->AddComponent<Transform>();
    float size = 11;
    camera->AddComponent<Camera>()->Viewport = Box(-size * Manager().Viewport().Aspect(), size, size * Manager().Viewport().Aspect(), -size);
    camera->GetComponent<Transform>()->Position = {0,10,20};
    camera->GetComponent<Camera>()->Orthographic = true;
    
    CreateLevel();
    GameObject* background = world.CreateObject();
    background->AddComponent<Transform>()->Position = {0,0,-0.5f};
    background->AddComponent<Mesh>()->AddQuad({0,10}, {10,19}, Colour::Black());
    background->AddComponent<Material>();
}

void Game::Update(float dt) {
    world.Update(dt);
}

void Game::Render() {
    glClearColor(0.3f, 0.3f, 1.0f, 1.0f); // Cornflower blue, XNA days
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world.Render();
}

void Game::CreateLevel() {
    
    for (int y=1; y<20; y++) {
        CreateBlock(-5, y);
    }
    
    for (int y=1; y<20; y++) {
        CreateBlock(5, y);
    }
    
    for (int x = -5; x<=5; x++) {
        CreateBlock(x,0);
    }
    
}

void Game::CreateBlock(int x, int y) {
    GameObject* go =  world.CreateObject();
    go->AddComponent<Transform>();
    go->AddComponent<Mesh>();
    go->AddComponent<Material>();
    go->AddComponent<Piece>()->grid[0][0] = true;
    go->AddComponent<PieceCollider>();
    go->GetComponent<Piece>()->wall = true;
    go->AddComponent<PieceStyle>()->color = Colour::White();
    
    PointTransform* pointTransform = go->AddComponent<PointTransform>();
    pointTransform->position = Point(x,y);
}
