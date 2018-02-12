//
//  Context.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorContext.hpp"
#include "Cloner.hpp"
#include "CloneVariable.hpp"
#include "AssetManager.hpp"
#include "EditorDropTarget.hpp"
#include "SystemHelper.hpp"
#include "FilePath.hpp"
#include "GameWorldViewportSystem.hpp"
#include "DragSelector.hpp"

GameStorage& EditorContext::Storage() { return storage; }
GameWorld& EditorContext::World() { return world; }
FileWorld& EditorContext::FileWorld() { return fileWorld; }
ScriptWorld& EditorContext::ScriptWorld() { return scriptWorld; }
GameObject& EditorContext::ContextRoot() { return *contextRoot; }
GameWorld& EditorContext::GuiWorld() { return guiWorld; }
GameObject& EditorContext::GuiRoot() { return *guiScene; }
Gui& EditorContext::Gui() { return *gui; }
EngineContext& EditorContext::EngineContext() { return *engineContext; }
Project& EditorContext::Project() { return project; }

void EditorContext::Initialize(class EngineContext& engineContext) {
    this->engineContext = &engineContext;
    
    world.Initialize(storage);
    guiWorld.Initialize(storage);
    
    storage.CreateSerializer<GameObjectJsonSerializer>();
    
    
    
    
    SystemHelper::AddGameSystems(storage);
    SystemHelper::AddEditorSystems(storage);
    
    fileWorld.Initialize(storage);
    fileWorld.OnRootCreated = [this] (GameObject* root) {
        root->GetSystem<AssetManager>()->SetFileWatcher(Project().FileSystemWatcher());
    };
    
    guiScene = guiWorld.CreateScene();
    
    gui = guiScene->GetSystem<class Gui>();
    guiScene->GetSystem<TouchSystem>()->TouchDepth = 10;
    guiScene->GetSystem<TouchSystem>()->Order = -200;
    guiScene->GetSystem<DragSelector>()->Setup(engineContext.Viewport());
    
    gui->Setup("NewUI.tga", "NewUI.json", engineContext.Viewport());
    gui->CreateFont("SanFranciscoText-Bold.otf");//, "Font");

    guiScene->GetSystem<RenderSystem>()->Order = 10;
    
    contextRoot = world.CreateScene();
    contextRoot->Hierarchy().Order = -100;
    
    project.Initialize(storage, fileWorld, scriptWorld);
    
    logRoot = world.CreateScene();
    //log = logRoot->CreateSystem<LogSystem>();
}

void EditorContext::Update(float dt) {
    //engineContext->InputDevice().UpdateInputManager(&world.Input());
    engineContext->InputDevice().UpdateInputManager(&guiWorld.Input());
    UpdateLoop(dt);
    DoActions(preActions);
    //world.Update(dt);
    guiWorld.Update(dt);
    DoActions(postActions);
    if (Project().Worlds.ActiveWorld()) {
        Project().Worlds.ActiveWorld()->Update(engineContext->InputDevice(), dt);
    }
    Project().Update();
}

void EditorContext::Render() {
//    if (Project().Worlds.ActiveWorld()) {
//        Project().Worlds.ActiveWorld()->Render();
//    }
    //world.Render();
    guiWorld.Render();
}

void EditorContext::DoActions(Actions& actions) {
    for (int i=0; i<actions.size(); ++i) {
        actions[i]();
    }
    actions.clear();
}

LogSystem& EditorContext::Log() { return *log; }
