//
//  Game.h
//  Cubism
//
//  Created by Jeppe Nielsen on 9/7/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameState.hpp"
#include "PuzzleDatabase.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"
#include "PuzzleProgress.hpp"
#include "FileDownloader.hpp"

using namespace Pocket;

class Game : public GameState<Game> {
  
public:
    void Initialize();
    void Update(float dt);
    void Render();
    
    PuzzleDatabase puzzleDatabase;
    PuzzleProgress puzzleProgress;
    
    GameWorld world;
    Gui* gui;
    GameObject* font;
    GameObject* animations;
    GameObject* menuObject;
    Menu* menu;
    
    GameWorld background;
    
    GameObject* CreateButton(Pocket::GameObject *parent, const std::string &spriteName, const Pocket::Vector2 &position, const Pocket::Vector2 &size, std::string text, float fontSize);
    
    FileDownloader downloader;
    
    void WriteLevelData();
    
private:
    void DownloadComplete(Pocket::DownloadedFile file);
    void ButtonDown(std::string button);
    
};