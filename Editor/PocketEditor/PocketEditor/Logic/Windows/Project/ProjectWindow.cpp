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
    factory = world.CreateSystem<SelectableFactory>();
    selectables = world.CreateSystem<SelectableCollection>();

    GameWorld& guiWorld = context->GuiWorld();
    guiWorld.CreateSystem<VirtualTreeListSystem>();
    guiWorld.CreateSystem<VirtualTreeListSpawnerSystem>();
}

void ProjectWindow::OnCreate() {
    GameWorld& world = context->World();
    Gui& gui = context->Gui();
    
    {
        GameObject* button = CreateButton(window, 10, {100,50}, "New");
        button->GetComponent<Touchable>()->Click.Bind([this](auto d) {
           auto object = context->Project().World().CreateObject();
           object->AddComponent<Transform>()->Position = {0,0,0};
           object->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
           object->AddComponent<Material>();
        });
    }
    
    {
        GameObject* button = CreateButton(window, {120,10}, {100,50}, "Delete");
        button->GetComponent<Touchable>()->Click.Bind([this](auto d) {
           for(auto o : selectables->Selected()) {
                GameObject* object = factory->GetGameObject(o);
                if (object) {
                    object->Remove();
                }
           }
        });
    }

    
}
