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
#include "EditorObject.hpp"

std::string ProjectWindow::Name() { return "Project"; }

void ProjectWindow::OnInitialize() {
    GameWorld& world = context->World();
    selectables = world.CreateSystem<SelectableCollection<EditorObject>>();

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
           object->AddComponent<EditorObject>();
        });
    }
    
    {
        GameObject* button = CreateButton(window, {120,10}, {100,50}, "Delete");
        button->GetComponent<Touchable>()->Click.Bind([this](auto d) {
           for(auto o : selectables->Selected()) {
                o->GetComponent<EditorObject>()->gameObject->Remove();
           }
        });
    }

     {
        GameObject* button = CreateButton(window, {120,70}, {100,50}, "Clone");
        button->GetComponent<Touchable>()->Click.Bind([this](auto d) {
           for(auto o : selectables->Selected()) {
                o->GetComponent<EditorObject>()->gameObject->Clone();
           }
        });
    }
    
}
