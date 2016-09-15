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
#include "FileSystemListenerSystem.hpp"

std::string ProjectWindow::Name() { return "Project"; }

void ProjectWindow::OnInitialize() {

    GameWorld& contextWorld = context->ContextWorld();
    contextWorld.CreateSystem<FileSystemListenerSystem>();
    
    GameWorld& guiWorld = context->GuiWorld();
    guiWorld.CreateSystem<VirtualTreeListSystem>();
    guiWorld.CreateSystem<VirtualTreeListSpawnerSystem>();
    guiWorld.CreateSystem<SelectedColorerSystem>();
    
    context->EngineContext().ScreenSize.Changed.Bind(this, &ProjectWindow::ScreenSizeChanged);
}

void ProjectWindow::OnCreate() {
    GameWorld& contextWorld = context->ContextWorld();
    
    GameObject* fileRoot = contextWorld.CreateObject();
    auto listener = fileRoot->AddComponent<FileSystemListener>();
    listener->Path = "/Projects/PocketEngine/Editor/TestProject/";
    listener->Extension = ".json";

    Gui& gui = context->Gui();

    GameObject* pivot;
    listBox = gui.CreateListbox(window, "Box", {0,0}, {200,window->GetComponent<Sizeable>()->Size().y-50}, &pivot);
    listBox->RemoveComponent<Sprite>();
    
    pivot->GetComponent<Transform>()->Position = {0,400};
    
    auto treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->Root = fileRoot;
    treeView->SetNodeExpanded(fileRoot, true);
    treeView->Pivot = listBox;
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    
    spawner->OnCreate = [&, fileRoot, this](GameObject* node, GameObject* parent) -> GameObject* {
        FilePath* filePath = node->GetComponent<FilePath>();
        GameObject* clone = gui.CreateControl(parent, "Box", {-1000,0}, {200,25});
        clone->RemoveComponent<Touchable>();
        gui.CreateControl(clone, "TextBox", 0, {25,25});
        
        if (node!=fileRoot) {
            GameObject* selectButton = gui.CreateControl(clone, "TextBox", {25,0}, {200-25,25});
            selectButton->GetComponent<Touchable>()->Click.Bind(this, &ProjectWindow::Clicked, node);
            
            if (filePath) {
                auto l = gui.CreateLabel(selectButton, {0,0}, {200-25,25}, 0, filePath->filename, 12);
                l->GetComponent<Colorable>()->Color = Colour::Black();
                l->GetComponent<Label>()->HAlignment = Font::Left;
                l->GetComponent<Label>()->VAlignment = Font::Middle;
            }
            
        } else {
        
        }
        return clone;
    };
    
    spawner->OnRemove = [] (GameObject* node, GameObject* control) {
        
    };
    
    ScreenSizeChanged();
}

bool ProjectWindow::CreateBar() { return false; }

void ProjectWindow::Clicked(TouchData d, GameObject* object) {
    std::cout << object->GetComponent<FilePath>()->path << std::endl;
    //object->GetComponent<Selectable>()->Selected = true;
    //factory->GetSelectable(object)->Selected = true;
    //object->GetComponent<Selectable>()->Selected = true;
    FilePath* filePath =object->GetComponent<FilePath>();
    context->Project().Worlds.LoadWorld(filePath->path, filePath->filename);
}

void ProjectWindow::ScreenSizeChanged() {
    Vector2 size = context->EngineContext().ScreenSize;
    window->GetComponent<Sizeable>()->Size = {200, size.y};
    window->GetComponent<Transform>()->Position = 0;
    listBox->GetComponent<Sizeable>()->Size = window->GetComponent<Sizeable>()->Size;
}

