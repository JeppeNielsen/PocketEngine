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
    GameWorld world;
    GameObject* contextRoot;
    GameObject* guiRoot;
    Gui* gui;
    EngineContext* engineContext;
    Project project;
    
public:
    GameWorld& World();
    GameObject& ContextRoot();
    GameObject& GuiRoot();
    Gui& Gui();
    EngineContext& EngineContext();
    Project& Project();
    
    void Initialize(class EngineContext& engineContext);
    void Update(float dt);
    void Render();
};
