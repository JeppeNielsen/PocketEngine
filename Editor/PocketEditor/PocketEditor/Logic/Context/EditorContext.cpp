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
FileWorld& EditorContext::FileWorld() { return fileWorld; }
ScriptWorld& EditorContext::ScriptWorld() { return scriptWorld; }
GameWorld& EditorContext::GuiWorld() { return guiWorld; }
GameObject& EditorContext::GuiRoot() { return *guiScene; }
Gui& EditorContext::Gui() { return *gui; }
EngineContext& EditorContext::EngineContext() { return *engineContext; }
Project& EditorContext::Project() { return project; }
LogSystem& EditorContext::Log() { return *logSystem; }

void EditorContext::Initialize(class EngineContext& engineContext) {
    this->engineContext = &engineContext;
    
    storage.PrefabLoaded = [this] (GameObject* prefab) {
        prefab->CreateSystem<AssetManager>()->SetFileWatcher(Project().FileSystemWatcher());
    };

    SystemHelper::InitializeStorage(storage);

    guiWorld.Initialize(storage);
    SystemHelper::SetScriptStorage(storage, scriptWorld);
    storage.CreateSerializer<GameObjectJsonSerializer>();
    
    fileWorld.Initialize(storage);
    
    guiScene = guiWorld.CreateScene();
    SystemHelper::AddGuiSystems(*guiScene);
    logSystem = guiScene->CreateSystem<LogSystem>();
    
    gui = guiScene->CreateSystem<class Gui>();
    guiScene->CreateSystem<TouchSystem>()->TouchDepth = 10;
    guiScene->CreateSystem<TouchSystem>()->Order = -200;
    
    gui->Setup("NewUI.tga", "NewUI.json", engineContext.Viewport());
    gui->CreateFont("SanFranciscoText-Bold.otf");//, "Font");

    guiScene->CreateSystem<RenderSystem>()->Order = 10;
    
    project.Initialize(storage, fileWorld, scriptWorld);
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
