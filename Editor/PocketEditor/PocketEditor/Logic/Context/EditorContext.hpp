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
#include "ScriptWorld.hpp"
#include <deque>

using namespace Pocket;

class EditorContext {
private:
    GameStorage storage;
    GameWorld world;
    FileWorld fileWorld;
    ScriptWorld scriptWorld;
    GameObject* contextRoot;
    GameWorld guiWorld;
    GameObject* guiScene;
    Gui* gui;
    EngineContext* engineContext;
    Project project;
    LogSystem* log;
    GameObject* logRoot;
public:
    GameStorage& Storage();
    GameWorld& World();
    FileWorld& FileWorld();
    ScriptWorld& ScriptWorld();
    GameObject& ContextRoot();
    GameWorld& GuiWorld();
    GameObject& GuiRoot();
    Gui& Gui();
    EngineContext& EngineContext();
    Project& Project();
    LogSystem& Log();
    using Action = std::function<void()>;
    using Actions = std::deque<Action>;
    Actions preActions;
    Actions postActions;
    
    Event<float> UpdateLoop;
    
    void Initialize(class EngineContext& engineContext);
    void Update(float dt);
    void Render();
private:
    void DoActions(Actions& actions);
};
