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

GameWorld& EditorContext::World() { return world; }
FileWorld& EditorContext::FileWorld() { return fileWorld; }
GameObject& EditorContext::ContextRoot() { return *contextRoot; }
GameWorld& EditorContext::GuiWorld() { return guiWorld; }
GameObject& EditorContext::GuiRoot() { return *guiRoot; }
Gui& EditorContext::Gui() { return *gui; }
EngineContext& EditorContext::EngineContext() { return *engineContext; }
Project& EditorContext::Project() { return project; }

void EditorContext::Initialize(class EngineContext& engineContext) {
    this->engineContext = &engineContext;
    
    fileWorld.AddGameWorld(world);
    fileWorld.OnRootCreated = [this] (GameObject* root) {
        root->CreateSystem<AssetManager>()->SetFileWatcher(Project().FileSystemWatcher());
    };
    fileWorld.OnChildCreated = [this] (GameObject* child) {
    //    child->AddComponent<EditorObject>();
    };
    
    
    guiRoot = guiWorld.CreateRoot();
    
    gui = guiRoot->CreateSystem<class Gui>();
    guiRoot->CreateSystem<TouchSystem>()->TouchDepth = 10;
    guiRoot->CreateSystem<TouchSystem>()->Order = -200;
    guiRoot->CreateSystem<GameWorldViewportSystem>();
    
    gui->Setup("NewUI.tga", "NewUI.json", engineContext.Viewport());
    gui->CreateFont("SanFranciscoText-Bold.otf");//, "Font");

    guiRoot->CreateSystem<RenderSystem>()->Order = 10;
    
    contextRoot = world.CreateRoot();
    contextRoot->Order = -100;
    
    world.AddComponentType<Cloner>();
    world.AddComponentType<CloneVariable>();
    world.AddComponentType<EditorDropTarget>();
    world.AddComponentType<ProjectSettings>();
    world.AddComponentType<FilePath>();
    
    GameObject* initRoot = world.CreateRoot();
    
    SystemHelper::AddGameSystems(*initRoot);
    SystemHelper::AddEditorSystems(*initRoot);
    
    initRoot->CreateSystem<AssetManager>();
    initRoot->Remove();
    
    project.Initialize(world);
    
    logRoot = world.CreateRoot();
    log = logRoot->CreateSystem<LogSystem>();
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
