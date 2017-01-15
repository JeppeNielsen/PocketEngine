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
#include "FileHelper.hpp"

#include <sys/stat.h>
#include <stdio.h>

std::string ProjectWindow::Name() { return "Project"; }

void ProjectWindow::OnInitialize() {

    GameObject& contextRoot = context->ContextRoot();
    contextRoot.CreateSystem<FileSystemListenerSystem>();
    
    GameObject& guiRoot = context->GuiRoot();
    guiRoot.CreateSystem<VirtualTreeListSystem>();
    guiRoot.CreateSystem<VirtualTreeListSpawnerSystem>();
    guiRoot.CreateSystem<SelectedColorerSystem>();
    guiRoot.CreateSystem<ClickSelectorSystem>();
    
    context->EngineContext().ScreenSize.Changed.Bind(this, &ProjectWindow::ScreenSizeChanged);
    
    popupMenu.InitializePopUp();
    {
        AppMenu& createMenu = popupMenu.AddChild("Create");
        createMenu.AddChild("Folder").Clicked.Bind([this]() {
            
            std::string folderPath = selectedNode.filePath->GetFolderPath() + "/New Folder";
            
            std::cout <<" Folder path: "<< folderPath<<std::endl;
            
            mkdir(folderPath.c_str(), 0777);
            fileSystemListener->watcher.Changed();
            
        });
        
        createMenu.AddChild("World").Clicked.Bind([this]() {
            std::string newWorldPath = selectedNode.filePath->GetFolderPath() + "/NewWorld.json";
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
            
            if (!selectedNode.guiNodeObject) return;
            
            Gui& gui = context->Gui();
            
            GameObject* textBox = gui.CreateTextBox(selectedNode.guiNodeObject, "TextBox", 0, selectedNode.guiNodeObject->GetComponent<Sizeable>()->Size, 0, selectedNode.filePath->filename, 12);
            GameObject* textBoxLabel = textBox->Children()[0];
            
            textBoxLabel->GetComponent<Label>()->HAlignment = Font::HAlignment::Left;
            textBoxLabel->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
            
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

void ProjectWindow::UpdateFileWorld() {
    context->FileWorld().FindRoots(fileSystemListener->Path, { ".json", ".meta" });
}

void ProjectWindow::OnCreate() {
    GameObject& contextRoot = context->ContextRoot();
    
    GameObject* fileRoot = contextRoot.CreateObject();
    fileSystemListener = fileRoot->AddComponent<FileSystemListener>();
    fileSystemListener->Extension = "";
    fileSystemListener->watcher.Changed.Bind([this] {
        UpdateFileWorld();
    });
    fileSystemListener->Path.Changed.Bind([this] {
        UpdateFileWorld();
    });

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
        
        GameObject* selectButton = gui.CreateControl(clone, "TextBox", {25,0}, {200-25,25});
        
        auto l = gui.CreateLabel(selectButton, {0,0}, {200-25,25}, 0, node!=fileRoot ? filePath->filename : "Project", 12);
        l->GetComponent<Colorable>()->Color = Colour::Black();
        l->GetComponent<Label>()->HAlignment = Font::HAlignment::Left;
        l->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
    
        selectButton->GetComponent<Touchable>()->Click.Bind(this, &ProjectWindow::Clicked, { node, node!=fileRoot ? selectButton : 0, filePath ? filePath : &projectFilePath });
        
        if (filePath) {
            if (node!=fileRoot) {
                selectButton->AddComponent<Selectable>();
                selectButton->AddComponent<SelectedColorer>()->Selected = Colour(0.8f,0.8f,0.8f,1.0f);
            }
        }
        return clone;
    };
    
    spawner->OnRemove = [] (GameObject* node, GameObject* control) {
        
    };
    
    ScreenSizeChanged();
    
    context->Project().Opened.Bind([this] () {
        SetProjectPath(context->Project().Path());
    });
}

bool ProjectWindow::CreateBar() { return false; }

void ProjectWindow::Clicked(TouchData d, ClickedNodeInfo nodeInfo) {
    selectedNode = nodeInfo;
    if (d.Index == 0) {
        if (!selectedNode.filePath->isFolder) {
            context->preActions.emplace_back([=]() {
            context->Project().Worlds.LoadWorld(selectedNode.filePath->GetFilePath(),
                                                selectedNode.filePath->filename,
                                                context);
            });
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

void ProjectWindow::SetProjectPath(const std::string &path) {
    projectFilePath.path = path;
    projectFilePath.isFolder = true;
    fileSystemListener->Path = path;
}
