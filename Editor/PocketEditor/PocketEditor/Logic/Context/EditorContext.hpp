//
//  Context.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Project.hpp"
#include "GameWorld.hpp"
#include "InputManager.hpp"
#include "Gui.hpp"

using namespace Pocket;

class EditorContext {
private:
    Project project;
    GameWorld world;
    Gui* gui;
    InputManager* input;
public:
    GameWorld& World();
    Gui& Gui();
    InputManager& Input();
    Project& Project();
    
    void Initialize(InputManager& input);
    void NewProject();
};
