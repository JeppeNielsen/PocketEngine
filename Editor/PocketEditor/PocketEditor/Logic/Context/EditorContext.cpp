//
//  Context.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorContext.hpp"

GameWorld& EditorContext::GuiWorld() { return guiWorld; }

Gui& EditorContext::Gui() { return *gui; }

Project& EditorContext::Project() { return project; }

EngineContext& EditorContext::EngineContext() {
    return *engineContext;
}

void EditorContext::Initialize(class EngineContext& engineContext) {
    this->engineContext = &engineContext;
    
    gui = guiWorld.CreateSystem<class Gui>();
    guiWorld.CreateSystem<TouchSystem>()->TouchDepth = 10;
    
    gui->Setup("images.png", "images.xml", {0,0, 1280,1024});
    gui->CreateFont("Font.fnt", "Font");
}

GameWorld& EditorContext::ContextWorld() { return contextWorld; }
