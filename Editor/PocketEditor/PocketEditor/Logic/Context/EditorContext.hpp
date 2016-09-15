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
#include "EngineContext.hpp"

using namespace Pocket;

class EditorContext {
private:
    Project project;
    GameWorld contextWorld;
    GameWorld guiWorld;
    Gui* gui;
    InputManager* input;
    EngineContext* engineContext;
public:
    GameWorld& ContextWorld();
    GameWorld& GuiWorld();
    Gui& Gui();
    InputManager& Input();
    Project& Project();
    EngineContext& EngineContext();
    
    void Initialize(InputManager& input, class EngineContext& engineContext);
    void NewProject();
};
