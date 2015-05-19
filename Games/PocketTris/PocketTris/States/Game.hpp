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

using namespace Pocket;

class Game : public GameState<Game> {
public:
    void Initialize();
    void Update(float dt);
    void Render();
private:
    GameWorld world;
    GameObject* camera;
    void CreateLevel();
    void CreateBlock(int x, int y);
};