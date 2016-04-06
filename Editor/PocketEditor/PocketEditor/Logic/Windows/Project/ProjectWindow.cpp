//
//  ProjectWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ProjectWindow.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"

std::string ProjectWindow::Name() { return "Project"; }

void ProjectWindow::OnInitialize() {

    GameWorld& world = context->World();

    world.CreateSystem<VirtualTreeListSystem>();
    world.CreateSystem<VirtualTreeListSpawnerSystem>();


}

void ProjectWindow::OnCreate() {
    GameWorld& world = context->World();
    Gui& gui = context->Gui();
}
