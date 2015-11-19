//
//  MenuState.h
//  Cubism
//
//  Created by Jeppe Nielsen on 9/7/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//
#pragma once

#include "Game.hpp"
#include "GameState.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"
#include <vector>

using namespace Pocket;

class MenuState : public GameState<Game> {
    
public:
    void Initialize();
    void Update(float dt);
    void Render();
    
   
    
    
    GameObject* CreateScrollPanel(GameObject* parent, float height);
    
    void LevelClicked(TouchData touch, int levelIndex);
    
    void UpdateLevelList();

    typedef std::vector<GameObject*> LevelObjects;
    LevelObjects levelObjects;
    
    void Enabled();
};