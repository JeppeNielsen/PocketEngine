//
//  Context.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorContext.hpp"

GameWorld& EditorContext::World() { return world; }
GameObject& EditorContext::ContextRoot() { return *contextRoot; }
GameObject& EditorContext::GuiRoot() { return *guiRoot; }
Gui& EditorContext::Gui() { return *gui; }
EngineContext& EditorContext::EngineContext() { return *engineContext; }
Project& EditorContext::Project() { return project; }

void EditorContext::Initialize(class EngineContext& engineContext) {
    this->engineContext = &engineContext;
    
    guiRoot = world.CreateRoot();
    guiRoot->Order = 1000;
    
    gui = guiRoot->CreateSystem<class Gui>();
    guiRoot->CreateSystem<TouchSystem>()->TouchDepth = 10;
    
    gui->Setup("images.png", "images.xml", engineContext.Viewport());
    gui->CreateFont("Font.fnt", "Font");

    contextRoot = world.CreateRoot();
    contextRoot->Order = 100;
    
    project.Initialize(world);
}

void EditorContext::Update(float dt) {
    engineContext->InputDevice().UpdateInputManager(&world.Input());
    world.Update(dt);
}

void EditorContext::Render() {
    world.Render();
}