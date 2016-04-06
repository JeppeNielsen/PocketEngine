//
//  Context.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorContext.hpp"

GameWorld& EditorContext::World() { return world; }

GameWorld& EditorContext::GuiWorld() { return guiWorld; }

InputManager& EditorContext::Input() { return *input; }

Gui& EditorContext::Gui() { return *gui; }

Project& EditorContext::Project() { return project; }

void EditorContext::NewProject() {
    project = { };
    project.CreateDefaultScene(world);
}

void EditorContext::Initialize(InputManager& input) {
    this->input = &input;
    
    gui = guiWorld.CreateSystem<class Gui>();
    
    gui->Setup("images.png", "images.xml", {0,0, 1280,1024}, input);
    gui->CreateFont("Font.fnt", "Font");
}
