//
//  Game.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameState.hpp"
#include "OpenGL.hpp"
#include "GameWorld.hpp"
#include "Camera.hpp"
#include <string>
#include <vector>
#include "Score.hpp"

using namespace Nano;

class Game : public GameState<Game> {
public:
    void Initialize();
    void Update(float dt);
    void Render();
    
private:
    
    void ButtonDown(std::string button);
    
    GameWorld* gameWorld;
    GameObject* camera;
    
    
    void CreateLevel();
    
    void CreateBlock(int x, int y);
    
    float rotation;
    
    std::vector<float> fps;
    
    Score* score;
    
    bool update;
    bool render;
    
    GameWorld guiWorld;
    
};
