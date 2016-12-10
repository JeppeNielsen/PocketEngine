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

GameWorld& EditorContext::World() { return world; }
FileWorld& EditorContext::FileWorld() { return fileWorld; }
GameObject& EditorContext::ContextRoot() { return *contextRoot; }
GameObject& EditorContext::GuiRoot() { return *guiRoot; }
Gui& EditorContext::Gui() { return *gui; }
EngineContext& EditorContext::EngineContext() { return *engineContext; }
Project& EditorContext::Project() { return project; }

void EditorContext::Initialize(class EngineContext& engineContext) {
    this->engineContext = &engineContext;
    
    fileWorld.AddGameWorld(world);
    fileWorld.OnRootCreated = [this] (GameObject* root) {
        
    };
    fileWorld.OnChildCreated = [this] (GameObject* child) {
    //    child->AddComponent<EditorObject>();
    };
    
    guiRoot = world.CreateRoot();
    
    gui = guiRoot->CreateSystem<class Gui>();
    guiRoot->CreateSystem<TouchSystem>()->TouchDepth = 10;
    guiRoot->CreateSystem<TouchSystem>()->Order = -200;
    
    gui->Setup("images.png", "images.xml", engineContext.Viewport());
    gui->CreateFont("Font.fnt", "Font");

    guiRoot->CreateSystem<RenderSystem>()->Order = 10;
    
    contextRoot = world.CreateRoot();
    contextRoot->Order = -100;
    
    world.AddComponentType<Cloner>();
    world.AddComponentType<CloneVariable>();
    
    GameObject* initRoot = world.CreateRoot();
    OpenWorld::CreateDefaultSystems(*initRoot);
    OpenWorld::CreateEditorSystems(*initRoot);
    initRoot->Remove();
    
    project.Initialize(world);
}

void EditorContext::Update(float dt) {
    engineContext->InputDevice().UpdateInputManager(&world.Input());
    DoActions(delayedActions);
    world.Update(dt);
}

void EditorContext::Render() {
    world.Render();
}

void EditorContext::DoActions(Actions& actions) {
    for (int i=0; i<actions.size(); ++i) {
        actions[i]();
    }
    actions.clear();
}