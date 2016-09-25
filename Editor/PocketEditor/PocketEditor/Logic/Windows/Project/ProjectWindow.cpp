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
#include "ClickSelectorSystem.hpp"

#include <sys/stat.h>
#include <stdio.h>

std::string ProjectWindow::Name() { return "Project"; }

void ProjectWindow::OnInitialize() {

    GameWorld& contextWorld = context->ContextWorld();
    contextWorld.CreateSystem<FileSystemListenerSystem>();
    
    GameWorld& guiWorld = context->GuiWorld();
    guiWorld.CreateSystem<VirtualTreeListSystem>();
    guiWorld.CreateSystem<VirtualTreeListSpawnerSystem>();
    guiWorld.CreateSystem<SelectedColorerSystem>();
    guiWorld.CreateSystem<ClickSelectorSystem>();
    
    context->EngineContext().ScreenSize.Changed.Bind(this, &ProjectWindow::ScreenSizeChanged);
    
    popupMenu.InitializePopUp();
    {
        AppMenu& createMenu = popupMenu.AddChild("Create");
        createMenu.AddChild("Folder").Clicked.Bind([this]() {
            
            std::string folderPath = selectedNode.filePath->GetFilePath() + "/New Folder";
            
            std::cout <<" Folder path: "<< folderPath<<std::endl;
            
            mkdir(folderPath.c_str(), 0777);
            fileSystemListener->watcher.Changed();
            
        });
        
        createMenu.AddChild("World").Clicked.Bind([this]() {
        
            std::string newWorldPath = selectedNode.filePath->GetFilePath() + "/NewWorld.json";
        
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
            FileReader::OpenPathInFileExplorer(selectedNode.filePath->path);
        });
    }
    {
        popupMenu.AddChild("Rename").Clicked.Bind([this] () {
            
            Gui& gui = context->Gui();
            
            GameObject* textBox = gui.CreateTextBox(selectedNode.guiNodeObject, "TextBox", 0, selectedNode.guiNodeObject->GetComponent<Sizeable>()->Size, 0, selectedNode.filePath->filename, 12);
            GameObject* textBoxLabel = textBox->Children()[0];
            
            textBoxLabel->GetComponent<Label>()->HAlignment = Font::Left;
            textBoxLabel->GetComponent<Label>()->VAlignment = Font::Middle;
            
            textBoxLabel->GetComponent<TextBox>()->Active = true;
            textBoxLabel->GetComponent<TextBox>()->Active.Changed.Bind([textBox, textBoxLabel, this] () {
                std::string oldPath = selectedNode.filePath->path +"/"+ selectedNode.filePath->filename;
                std::string newPath = selectedNode.filePath->path +"/"+ textBoxLabel->GetComponent<TextBox>()->Text;
                
                std::cout << "Rename from: " << oldPath << " to: " << newPath << std::endl;
                
                int result = rename(oldPath.c_str(), newPath.c_str());
                if (result == 0) {
                    textBox->Remove();
                    fileSystemListener->watcher.Changed();
                } else {
                    textBoxLabel->GetComponent<TextBox>()->Text = selectedNode.filePath->filename;
                }
            });
            
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
            
            
            if (filePath) {
                auto l = gui.CreateLabel(selectButton, {0,0}, {200-25,25}, 0, filePath->filename, 12);
                l->GetComponent<Colorable>()->Color = Colour::Black();
                l->GetComponent<Label>()->HAlignment = Font::Left;
                l->GetComponent<Label>()->VAlignment = Font::Middle;
                
                selectButton->GetComponent<Touchable>()->Click.Bind(this, &ProjectWindow::Clicked, { node, selectButton, 0 });
                selectButton->AddComponent<Selectable>();
                selectButton->AddComponent<SelectedColorer>()->Selected = Colour(0.8f,0.8f,0.8f,1.0f);
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

void ProjectWindow::Clicked(TouchData d, ClickedNodeInfo nodeInfo) {
    selectedNode = nodeInfo;
    selectedNode.filePath = nodeInfo.fileObject->GetComponent<FilePath>();
    if (d.Index == 0) {
        if (!selectedNode.filePath->isFolder) {
            context->Project().Worlds.LoadWorld(selectedNode.filePath->GetFilePath(), selectedNode.filePath->filename);
        }
    } else if (d.Index == 1) {
        popupMenu.ShowPopup(d.Input->GetTouchPosition(1));
    }
}

void ProjectWindow::ScreenSizeChanged() {
    Vector2 size = context->EngineContext().ScreenSize;
    window->GetComponent<Sizeable>()->Size = {200, size.y};
    window->GetComponent<Transform>()->Position = 0;
    listBox->GetComponent<Sizeable>()->Size = window->GetComponent<Sizeable>()->Size;
}

