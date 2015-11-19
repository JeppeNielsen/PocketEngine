//
//  Game.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 9/7/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Game.hpp"
#include "MenuState.hpp"
#include "PlayingState.hpp"
#include "RenderSystem.hpp"
#include "BlockMeshSystem.hpp"
#include "TransformAnimatorSystem.hpp"
#include "TouchAnimatorSystem.hpp"
#include "MathHelper.hpp"
#include "DraggableMotionSystem.hpp"
#include "VelocitySystem.hpp"
#include "LimitableSystem.hpp"
#include "TouchCancelSystem.hpp"
#include <sstream>

void Game::Initialize() {
    AddState<MenuState>("Menu");
    AddState<PlayingState>("Playing");
    CurrentState = "Menu";
    
    for (int i=0; i<30; i++) {
        Puzzle& puzzle = puzzleDatabase.AddPuzzle();
        puzzle.points.push_back({0,0,0});
        
        puzzle.seed = i;
        std::stringstream s;
        s<<"LEVEL "<<(i+1);
        puzzle.Name = s.str();
    }
    
    puzzleProgress.SetDatabase(&puzzleDatabase);
    
    world.CreateSystem<VelocitySystem>();
    gui = world.CreateFactory<Gui>();
    world.CreateSystem<TransformAnimatorSystem>();
    world.CreateSystem<TouchAnimatorSystem>();
    world.CreateSystem<DraggableMotionSystem>();
    
    world.CreateSystem<LimitableSystem>();
    world.CreateSystem<TouchCancelSystem>();
    
    gui->Setup("images.png", "images.xml", Manager().Viewport(), Input);
    
    font = gui->CreateFont("Font.fnt", "Font");
    
    animations = world.CreateObject();
    
    menuObject = gui->CreateMenu(0, 0);
    menu = menuObject->GetComponent<Menu>();
    menu->ActiveMenu = "Front";
    
    
    background.CreateSystem<RenderSystem>();
    background.CreateSystem<BlockMeshSystem>();
    
    GameObject* camObject = background.CreateObject();
    camObject->AddComponent<Transform>()->Position = {0,0,20};
    camObject->AddComponent<Camera>()->Viewport = Manager().Viewport();
    
    for (int i=0; i<10; i++) {
        
        GameObject* block = background.CreateObject();
        block->AddComponent<Block>()->CreateRandom();
        block->AddComponent<Transform>()->Position = {-10+i*5.0f,0,0};
        block->GetComponent<Transform>()->Scale = {1,1,0.001f};
        block->AddComponent<Mesh>();
        block->AddComponent<Material>();
        
        
        
    }
 
    Input.ButtonDown += event_handler(this, &Game::ButtonDown);
    
    downloader.DownloadComplete += event_handler(this, &Game::DownloadComplete);
    
    downloader.DownloadFile("http://www.pocketgames.dk/cubism/GetUserData.php?id=786786453543");//.Completed += event_handler(this, &Game::DownloadComplete);
}

void Game::DownloadComplete(Pocket::DownloadedFile file) {
    
    std::string json;
    json.append((char*)file.data, file.size);
    
    puzzleProgress.UpdateFromJSON(json);
    
    std::cout<< " json : "<<json <<std::endl;
}

void Game::WriteLevelData() {
    
    
}

GameObject* Game::CreateButton(Pocket::GameObject *parent, const std::string &spriteName, const Pocket::Vector2 &position, const Pocket::Vector2 &size, std::string text, float fontSize) {
    GameObject* button = gui->CreateLabelControl(parent, spriteName, position, size, font, text, fontSize);
    button->AddComponent<TransformAnimator>();
    button->AddComponent<TransformAnimationDatabase>(animations);
    button->AddComponent<TouchAnimator>()->DownAnimation = "Down";
    button->GetComponent<TouchAnimator>()->UpAnimation = "Up";
    return button;
}

void Game::Update(float dt) {
    background.Update(dt);
    world.Update(dt);
}

void Game::Render() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    background.Render();
}

void Game::ButtonDown(std::string button) {
    if (button!=" ") return;
    if (CurrentState() == "Menu") {
        CurrentState = "Playing";
    } else {
        CurrentState = "Menu";
    }
}

