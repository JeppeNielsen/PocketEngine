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
#include "Gui.hpp"
#include "ScoreLabelSystem.hpp"


void Game::Initialize() {
    
    Gui* gui = guiWorld.CreateFactory<Gui>();
    gui->Setup("images.png", "images.xml", Manager().Viewport(), Input);
    guiWorld.CreateSystem<ScoreLabelSystem>();
    
    GameObject* font = gui->CreateFont("OtherFont.fnt", "OtherFont");
    GameObject* label = gui->CreateLabel(0, {10, Manager().Viewport().top - 10 }, 0, font, "Score", 20);
    score = label->AddComponent<Score>();
    
    gameWorld = new GameWorld();
    
    GameWorld& world = *gameWorld;
    
    world.CreateSystem<RenderSystem>();
    
    PieceSpawnerSystem* spawner = world.CreateSystem<PieceSpawnerSystem>();
    world.CreateSystem<PieceControllerSystem>()->SetInputManager(Input);
    world.CreateSystem<PointGravitySystem>();
    world.CreateSystem<PieceCollisionSystem>();
    world.CreateSystem<PointTransformSystem>();
    world.CreateSystem<PieceDestructorSystem>();
    world.CreateSystem<PieceLineDetectorSystem>()->SetScore(score);
    world.CreateSystem<PieceMeshSystem>();
    world.CreateSystem<PieceVelocitySystem>();
    
    spawner->CreatePieces();
    
    
    camera = world.CreateObject();
    camera->AddComponent<Transform>();
    float size = 10;
    camera->AddComponent<Camera>()->Viewport = Box(-size * Manager().Viewport().Aspect(), size, size * Manager().Viewport().Aspect(), -size);
    camera->GetComponent<Transform>()->Position = {0,10,20};
    camera->GetComponent<Camera>()->Orthographic = true;
    //camera->GetComponent<Transform>()->Rotation = Quaternion::LookAt(camera->GetComponent<Transform>()->Position, {0,10,0}, {0,1,0});
    
    CreateLevel();
    GameObject* background = world.CreateObject();
    background->AddComponent<Transform>()->Position = {0,0,-0.5f};
    background->AddComponent<Mesh>()->AddQuad({0,10}, {10,19}, Colour::Black());
    background->AddComponent<Material>();
    
    Input.ButtonDown += event_handler(this, &Game::ButtonDown);
    
    update = true;
    render = true;
}

void Game::ButtonDown(std::string button) {
    if (button=="u") {
        update = !update;
    } else if (button == "r") {
        render = !render;
    }
}

void Game::Update(float dt) {

    fps.push_back(1/dt);
    
    if (fps.size()>60) {
        float sum = 0;
        for (int i=0; i<fps.size(); i++) {
            sum += fps[i];
        }
        sum /= fps.size();
        std::cout<<"FPS :" << sum<<std::endl;
        fps.clear();
    }
    
    if (update) {
        gameWorld->Update(dt);
        guiWorld.Update(dt);
    }
}

void Game::Render() {

    glClearColor(0.3f, 0.3f, 1.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (render) {
        gameWorld->Render();
        guiWorld.Render();
    }

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
    GameObject* go =  gameWorld->CreateObject();
    go->AddComponent<Transform>();
    go->AddComponent<Mesh>();
    go->AddComponent<Material>();
    go->AddComponent<Piece>()->grid[0][0] = true;
    go->AddComponent<PieceCollider>();
    go->GetComponent<Piece>()->wall = true;
    go->AddComponent<PieceStyle>()->color = Colour::HslToRgb(x * 60 + y * 60, 1, 1, 1);
    
    PointTransform* pointTransform = go->AddComponent<PointTransform>();
    pointTransform->position = Point(x,y);
}
