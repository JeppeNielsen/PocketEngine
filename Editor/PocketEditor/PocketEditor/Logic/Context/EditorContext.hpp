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
#include "ScriptWorld.hpp"

using namespace Pocket;

class EditorContext {
private:
    Project project;
    GameWorld contextWorld;
    GameWorld guiWorld;
    Gui* gui;
    InputManager* input;
public:
    GameWorld& ContextWorld();
    GameWorld& GuiWorld();
    Gui& Gui();
    InputManager& Input();
    Project& Project();
    
    void Initialize(InputManager& input);
    void NewProject();
};
