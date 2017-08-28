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

    Gui& gui = context->Gui();
    
    GameObject* pivot;
    listBox = gui.CreateListbox(window, "Window", {1,0}, 200, &pivot);
    listBox->RemoveComponent<Renderable>();
    
    gui.AddLayouter(listBox, 20, {2000,2000}, {2000,2000});
    
    pivot->GetComponent<Transform>()->Position = {0,400};
    
    treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->Root = fileRoot;
    treeView->SetNodeExpanded(fileRoot, true);
    treeView->Pivot = listBox;
    treeView->ExpandedHashFunction = [this] (GameObject* go) {
        FilePath* filePath = go->GetComponent<FilePath>();
        if (filePath)  {
            return filePath->GetFilePath();
        } else {
            return treeView->DefaultExpandedHashFunction(go);
        }
    };
    treeView->ItemHeight = 18.0f;
    
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    
    static int colorIndex = 0;
    
    spawner->OnCreate = [&, this](auto& n) {
        
        const float height = 18.0f;
        FilePath* filePath = n.node->template GetComponent<FilePath>();
        
        GameObject* selectButton = gui.CreateControl(n.parent, "white");
        selectButton->AddComponent<TouchableCanceller>();
        selectButton->GetComponent<Touchable>()->ClickThrough = true;
        selectButton->ReplaceComponent<Sizeable>(n.parent);
        gui.AddLayouter(selectButton, {4,height}, {2000,height}, {2000,height}, Layouter::LayoutMode::Horizontal);
        
        Vector2 startPos = { 8.0f + n.depth * 16.0f , 0 };
        if (n.hasChildren) {
        
            bool isUnfolded = treeView->IsNodeExpanded(n.node);
        
            Vector2 s = isUnfolded ? Vector2(9.0f, 7.5f) : Vector2(7.5f, 9.0f);
            n.foldOutButton = gui.CreateControl(selectButton, isUnfolded ? "Icon_arrowDown" : "Icon_arrowRight",startPos + Vector2(0, height / 2 - s.y /2),s);
            
            s = {16.0f, 12.0f};
            gui.CreateControl(selectButton, "Icon_folder", startPos + Vector2(15.0f, height / 2 - s.y/2),s);
        } else {
            Vector2 s = {13.0f, 16.0f};
            gui.CreateControl(selectButton, "Icon_file", startPos + Vector2(15.0f, height / 2 - s.y/2),s);
        }
        
        auto l = gui.CreateLabel(selectButton, startPos + Vector2(40.0f, 0), {200, height}, nullptr, n.node!=fileRoot ? filePath->filename : context->Project().GetFolderName(), 12);
        l->GetComponent<Colorable>()->Color = Colour::Black();
        l->GetComponent<Label>()->HAlignment = Font::HAlignment::Left;
        l->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
    
        selectButton->GetComponent<Touchable>()->Click.Bind(this, &ProjectWindow::Clicked, { n.node, n.node!=fileRoot ? selectButton : 0, filePath ? filePath : &projectFilePath });
        
        if (filePath) {
            if (n.node!=fileRoot) {
                selectButton->AddComponent<Selectable>()->Selected.Changed.Bind([selectButton, l] {
                    l->GetComponent<Colorable>()->Color = selectButton->GetComponent<Selectable>()->Selected() ?
                    Colour::White() : Colour::Black();
                });
                
                selectButton->AddComponent<SelectedColorer>()->Deselected = n.position % 2 == 0 ? Colour((Colour::Component)245,245,245,255) : Colour((Colour::Component)255,255,255,255);
                selectButton->AddComponent<SelectedColorer>()->Selected = Colour((Colour::Component)17,108,214,255);
                
            }
        }
        
        colorIndex++;
    };
    
    spawner->OnRemove = [] (auto& n) {};
    
    spawner->OnFoldoutChanged = [this] (auto& n) {
        bool isUnfolded = treeView->IsNodeExpanded(n.node);
        n.foldOutButton->template GetComponent<Sprite>()->SpriteName = isUnfolded ? "Icon_arrowDown" : "Icon_arrowRight";
        Vector2 s = isUnfolded ? Vector2(9.0f, 7.5f) : Vector2(7.5f, 9.0f);
        n.foldOutButton->template GetComponent<Sizeable>()->Size = s;
    };
    
    ScreenSizeChanged();
    
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

void ProjectWindow::ScreenSizeChanged() {
//    Vector2 size = context->EngineContext().ScreenSize;
//    window->GetComponent<Sizeable>()->Size = {200, size.y};
//    window->GetComponent<Transform>()->Position = 0;
//    listBox->GetComponent<Sizeable>()->Size = window->GetComponent<Sizeable>()->Size;
}

void ProjectWindow::SetProjectPath(const std::string &path) {
    projectFilePath.path = path;
    projectFilePath.isFolder = true;
    treeView->Clear();
    fileSystemListener->Path = path;
    fileSystemListener->watcher.Changed();
}
