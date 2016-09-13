//
//  HierarchyWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "HierarchyWindow.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"
#include "SelectedColorer.hpp"
#include "Selectable.hpp"

std::string HierarchyWindow::Name() { return "Hierarchy"; }

void HierarchyWindow::OnInitialize() {
    GameWorld& guiWorld = context->GuiWorld();
    guiWorld.CreateSystem<VirtualTreeListSystem>();
    guiWorld.CreateSystem<VirtualTreeListSpawnerSystem>();
    guiWorld.CreateSystem<SelectedColorerSystem>();
}

void HierarchyWindow::OnCreate() {
    Gui& gui = context->Gui();

    return;
    GameObject* pivot;
    GameObject* listBox = gui.CreateListbox(window, "Box", {0,0}, {200,window->GetComponent<Sizeable>()->Size().y-50}, &pivot);
    listBox->RemoveComponent<Sprite>();
    
    GameObject* root = (GameObject*)context->Project().World().Root();

    auto treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->Root = root;
    treeView->SetNodeExpanded(root, true);
    treeView->Pivot = listBox;
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    
    spawner->OnCreate = [&, root, this](GameObject* node, GameObject* parent) -> GameObject* {
        GameObject* clone = gui.CreateControl(parent, "Box", {-1000,0}, {200,25});
        clone->RemoveComponent<Touchable>();
        gui.CreateControl(clone, "TextBox", 0, {25,25});
        if (node!=root) {
            EditorObject* editorObject = node->GetComponent<EditorObject>();
            if (editorObject && editorObject->editorObject) {
                GameObject* selectButton = gui.CreateControl(clone, "TextBox", {25,0}, {200-25,25});
                selectButton->GetComponent<Touchable>()->Click.Bind(this, &HierarchyWindow::Clicked, editorObject->editorObject);
                selectButton->AddComponent<Selectable>(editorObject->editorObject);
                selectButton->AddComponent<SelectedColorer>()->Selected = Colour::Blue();
            }
            
        } else {
            std::cout << "yes"<<std::endl;
        }
        
        return clone;
    };
    
    spawner->OnRemove = [] (GameObject* node, GameObject* control) {
        
    };
}

void HierarchyWindow::Clicked(TouchData d, GameObject* object) {
    auto selectables = context->Project().GetSelectables();
    selectables->ClearSelection();
    object->GetComponent<Selectable>()->Selected = true;
    //factory->GetSelectable(object)->Selected = true;
    //object->GetComponent<Selectable>()->Selected = true;
}


