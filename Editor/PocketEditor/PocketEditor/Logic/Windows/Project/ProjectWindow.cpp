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
#include "AssetHelper.hpp"
#include "GameObjectFieldEditor.hpp"

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
    editorGui = guiRoot.CreateSystem<EditorGui>();

    popupMenu.InitializePopUp();
    {
        AppMenu& createMenu = popupMenu.AddChild("Create");
        createMenu.AddChild("Folder").Clicked.Bind([this]() {
            AssetHelper::TryCreateAsset(selectedNode.filePath->GetFolderPath(), "Create Folder", "New Folder", "", [this] (const std::string& path) {
                if (AssetHelper::CreateFolder(path)) {
                    fileSystemListener->watcher.Changed();
                }
            });
        });
        
        createMenu.AddChild("Prefab").Clicked.Bind([this]() {
            AssetHelper::TryCreateAsset(selectedNode.filePath->GetFolderPath(), "Create Prefab", "Source", ".json", [this] (const std::string& path) {
                if (AssetHelper::CreateEmptyPrefab(path)) {
                    fileSystemListener->watcher.Changed();
                }
            });
        });
        
        createMenu.AddChild("Scene").Clicked.Bind([this]() {
            AssetHelper::TryCreateAsset(selectedNode.filePath->GetFolderPath(), "Create Scene", "Scene", ".json", [this] (const std::string& path) {
                if (AssetHelper::CreateScene(path)) {
                    fileSystemListener->watcher.Changed();
                }
            });
        });
        
        createMenu.AddChild("Component").Clicked.Bind([this]() {
            AssetHelper::TryCreateAsset(selectedNode.filePath->GetFolderPath(), "Create Component Source File", "Component", "", [this] (const std::string& path) {
                if (AssetHelper::CreateComponent(path)) {
                    fileSystemListener->watcher.Changed();
                }
            });
        });

        createMenu.AddChild("C++ Header File").Clicked.Bind([this]() {
            AssetHelper::TryCreateAsset(selectedNode.filePath->GetFolderPath(), "Create C++ Header", "Header", ".hpp", [this] (const std::string& path) {
                if (AssetHelper::CreateHeader(path)) {
                    fileSystemListener->watcher.Changed();
                }
            });
        });
        
        createMenu.AddChild("C++ Source File").Clicked.Bind([this]() {
            AssetHelper::TryCreateAsset(selectedNode.filePath->GetFolderPath(), "Create C++ Source", "Source", ".cpp", [this] (const std::string& path) {
                if (AssetHelper::CreateSource(path)) {
                    fileSystemListener->watcher.Changed();
                }
            });
        });
        createMenu.AddChild("Shader").Clicked.Bind([this]() {
            AssetHelper::TryCreateAsset(selectedNode.filePath->GetFolderPath(), "Create Shader", "Shader", ".shader", [this] (const std::string& path) {
                if (AssetHelper::CreateShader(path)) {
                    fileSystemListener->watcher.Changed();
                }
            });
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
                    context->postActions.emplace_back([=] () {
                        textBoxLabel->GetComponent<TextBox>()->Active.Changed.Clear();
                        textBox->Remove();
                    });
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
    
    fileRoot = contextRoot.CreateObject();
    fileSystemListener = fileRoot->AddComponent<FileSystemListener>();
    fileSystemListener->Extension = "";
    fileSystemListener->watcher.Changed.Bind([this] {
        UpdateFileWorld();
    });
    fileSystemListener->Path.Changed.Bind([this] {
        UpdateFileWorld();
    });
    
    GameObject* treeViewGo = editorGui->CreateTreeList(window, fileRoot,
    
        [this] (GameObject* go) {
            FilePath* filePath = go->GetComponent<FilePath>();
            if (filePath)  {
                return filePath->GetFilePath();
            } else {
                return treeView->DefaultExpandedHashFunction(go);
            }
        },
        nullptr, nullptr,
        [this] (auto& n, GameObject* button, std::string& text) {
            FilePath* filePath = n.node->template GetComponent<FilePath>();
            text = n.node!=fileRoot ? filePath->filename : context->Project().GetFolderName();
            
            button->GetComponent<Touchable>()->Click.Bind(this, &ProjectWindow::Clicked, { n.node, n.node!=fileRoot ? button : 0, filePath ? filePath : &projectFilePath });
            
            button->AddComponent<Droppable>()->OnCreate = [text, this](GameObject* o, TouchData d) -> GameObject* {
                Gui& gui = context->Gui();
                Vector3 position = o->GetComponent<Transform>()->World().TransformPosition(0);
                GameObject* control = gui.CreateControl(0, "Box", position, {200,25});
                GameObject* label = gui.CreateLabel(control, 0, {200,25}, nullptr, text, 20);
                label->GetComponent<Colorable>()->Color = Colour::Black();
                label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
                label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
                
                return control;
            };
            
            button->AddComponent<Droppable>()->Dropped.Bind(this, &ProjectWindow::Dropped, n.node);
        },
        [this] (auto& n, GameObject* button) {
        
        }, true);
    
    treeView = treeViewGo->GetComponent<VirtualTreeList>();
    
    context->Project().Opened.Bind([this] () {
        SetProjectPath(context->Project().Path());
    });
}

bool ProjectWindow::CreateBar() { return true; }

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

void ProjectWindow::SetProjectPath(const std::string &path) {
    projectFilePath.path = path;
    projectFilePath.isFolder = true;
    treeView->Clear();
    fileSystemListener->Path = path;
    fileSystemListener->watcher.Changed();
}


void ProjectWindow::Dropped(Pocket::DroppedData d, Pocket::GameObject *object) {

    FilePath* filePath = object->GetComponent<FilePath>();
    if (!filePath || filePath->isFolder) {
        return;
    }
    
    //text = n.node!=fileRoot ? filePath->filename : context->Project().GetFolderName();
    
    for(auto& t : d.droppedTouches) {
        GameObjectFieldEditor* goField = t.object->GetComponent<GameObjectFieldEditor>();
        if (goField && goField->SetObject) {
        
            std::ifstream file;
        
            file.open(filePath->GetFilePath());
            std::string guid = context->World().ReadGuidFromJson(file);
            file.close();
            
            GameObject* root = context->World().TryFindRoot(guid);
            if (root) {
                goField->SetObject(root);
                return;
            }
        }
    }
}

