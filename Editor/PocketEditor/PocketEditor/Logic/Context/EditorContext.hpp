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
#include "FileWorld.hpp"
#include "LogSystem.hpp"
#include <deque>

using namespace Pocket;

class EditorContext {
private:
    GameWorld world;
    FileWorld fileWorld;
    GameObject* contextRoot;
    GameObject* guiRoot;
    Gui* gui;
    EngineContext* engineContext;
    Project project;
    LogSystem* log;
    GameObject* logRoot;
public:
    GameWorld& World();
    FileWorld& FileWorld();
    GameObject& ContextRoot();
    GameObject& GuiRoot();
    Gui& Gui();
    EngineContext& EngineContext();
    Project& Project();
    LogSystem& Log();
    using Action = std::function<void()>;
    using Actions = std::deque<Action>;
    Actions preActions;
    Actions postActions;
    
    void Initialize(class EngineContext& engineContext);
    void Update(float dt);
    void Render();
private:
    void DoActions(Actions& actions);
};
