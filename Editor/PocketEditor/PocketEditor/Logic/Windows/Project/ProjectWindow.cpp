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
#include "FileReader.hpp"

#include <sys/stat.h>

std::string ProjectWindow::Name() { return "Project"; }

void ProjectWindow::OnInitialize() {

    GameWorld& contextWorld = context->ContextWorld();
    contextWorld.CreateSystem<FileSystemListenerSystem>();
    
    GameWorld& guiWorld = context->GuiWorld();
    guiWorld.CreateSystem<VirtualTreeListSystem>();
    guiWorld.CreateSystem<VirtualTreeListSpawnerSystem>();
    guiWorld.CreateSystem<SelectedColorerSystem>();
    
    context->EngineContext().ScreenSize.Changed.Bind(this, &ProjectWindow::ScreenSizeChanged);
    
    popupMenu.InitializePopUp();
    {
        AppMenu& createMenu = popupMenu.AddChild("Create");
        createMenu.AddChild("Folder").Clicked.Bind([this]() {
            
            std::string folderPath = selectedPath->path + "/New Folder";
            
            std::cout <<" Folder path: "<< folderPath<<std::endl;
            
            mkdir(folderPath.c_str(), 0777);
            fileSystemListener->watcher.Changed();
            
        });
        
        createMenu.AddChild("World").Clicked.Bind([this]() {
        
            std::string newWorldPath = selectedPath->path + "/NewWorld.json";
        
            std::cout <<" Folder path: "<< newWorldPath<<std::endl;
        
            context->Project().CreateNewWorld(newWorldPath);
            
            fileSystemListener->watcher.Changed();
        });
        
        createMenu.AddChild("Header File").Clicked.Bind([this]() {
        
        });
        
        createMenu.AddChild("C++ File").Clicked.Bind([this] () {
        
        });
        
    }
    {
        popupMenu.AddChild("Reveal in Finder").Clicked.Bind([this] () {
            FileReader::OpenPathInFileExplorer(selectedPath->path);
        });
    }
    
}

void ProjectWindow::OnCreate() {
    GameWorld& contextWorld = context->ContextWorld();
    
    GameObject* fileRoot = contextWorld.CreateObject();
    fileSystemListener = fileRoot->AddComponent<FileSystemListener>();
    fileSystemListener->Path = "/Projects/PocketEngine/Editor/TestProject/";
    fileSystemListener->Extension = "";

    Gui& gui = context->Gui();

    GameObject* pivot;
    listBox = gui.CreateListbox(window, "Box", {0,0}, {200,window->GetComponent<Sizeable>()->Size().y-50}, &pivot);
    listBox->RemoveComponent<Sprite>();
    
    pivot->GetComponent<Transform>()->Position = {0,400};
    
    auto treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->Root = fileRoot;
    treeView->SetNodeExpanded(fileRoot, true);
    treeView->Pivot = listBox;
    treeView->ExpandedHashFunction = [treeView] (GameObject* go) {
        FilePath* filePath = go->GetComponent<FilePath>();
        if (filePath)  {
            return filePath->GetFilePath();
        } else {
            return treeView->DefaultExpandedHashFunction(go);
        }
    };
    
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
    std::cout << object->GetComponent<FilePath>()->GetFilePath() << std::endl;
    //object->GetComponent<Selectable>()->Selected = true;
    //factory->GetSelectable(object)->Selected = true;
    //object->GetComponent<Selectable>()->Selected = true;
    FilePath* filePath =object->GetComponent<FilePath>();
    if (d.Index == 0) {
        if (!filePath->isFolder) {
            context->Project().Worlds.LoadWorld(filePath->GetFilePath(), filePath->filename);
        }
    } else if (d.Index == 1) {
        selectedPath = filePath;
        popupMenu.ShowPopup(d.Input->GetTouchPosition(1));
    }
}

void ProjectWindow::ScreenSizeChanged() {
    Vector2 size = context->EngineContext().ScreenSize;
    window->GetComponent<Sizeable>()->Size = {200, size.y};
    window->GetComponent<Transform>()->Position = 0;
    listBox->GetComponent<Sizeable>()->Size = window->GetComponent<Sizeable>()->Size;
}

