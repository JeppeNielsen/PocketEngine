//
//  Context.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorContext.hpp"

GameWorld& EditorContext::GuiWorld() { return guiWorld; }

InputManager& EditorContext::Input() { return *input; }

Gui& EditorContext::Gui() { return *gui; }

Project& EditorContext::Project() { return project; }

void EditorContext::NewProject() {
    project.CreateNew("/Projects/PocketEngine/Editor/TestProject/");
}

EngineContext& EditorContext::EngineContext() {
    return *engineContext;
}

void EditorContext::Initialize(InputManager& input, class EngineContext& engineContext) {
    this->input = &input;
    this->engineContext = &engineContext;
    
    gui = guiWorld.CreateSystem<class Gui>();
    guiWorld.CreateSystem<TouchSystem>()->TouchDepth = 10;
    
    gui->Setup("images.png", "images.xml", {0,0, 1280,1024}, input);
    gui->CreateFont("Font.fnt", "Font");
    
    project.Initialize(input);
}

GameWorld& EditorContext::ContextWorld() { return contextWorld; }
