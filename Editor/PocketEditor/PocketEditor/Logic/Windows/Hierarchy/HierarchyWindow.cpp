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
#include "DroppableSystem.hpp"

std::string HierarchyWindow::Name() { return "Hierarchy"; }

void HierarchyWindow::OnInitialize() {
    GameObject& guiRoot = context->GuiRoot();
    guiRoot.CreateSystem<VirtualTreeListSystem>();
    guiRoot.CreateSystem<VirtualTreeListSpawnerSystem>();
    guiRoot.CreateSystem<SelectedColorerSystem>();
    guiRoot.CreateSystem<DraggableSystem>();
}

void HierarchyWindow::ActiveWorldChanged(OpenWorld* old, OpenWorld* current) {
    rootItem = 0;
    if (current) {
         treeView->Root = (GameObject*)current->Root();
    } else {
         treeView->Root = 0;
    }
}

void HierarchyWindow::OnCreate() {
    Gui& gui = context->Gui();

    GameObject* pivot;
    GameObject* listBox = gui.CreateListbox(window, "Box", {0,0}, {200,window->GetComponent<Sizeable>()->Size().y-50}, &pivot);
    listBox->RemoveComponent<Sprite>();

    treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->Root = 0;
    //treeView->SetNodeExpanded(root, true);
    treeView->Pivot = listBox;
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    
    spawner->OnCreate = [&, this](GameObject* node, GameObject* parent) -> GameObject* {
        GameObject* clone = gui.CreateControl(parent, "Box", {-1000,0}, {200,25});
        clone->AddComponent<Droppable>()->Dropped.Bind(this, &HierarchyWindow::Dropped, node);
        gui.CreateControl(clone, "TextBox", 0, {25,25});
        
        clone->GetComponent<Droppable>()->OnCreate = [&gui, this](GameObject* o, TouchData d) -> GameObject* {
            Vector3 position = o->GetComponent<Transform>()->World().TransformPosition(0);
            GameObject* control = gui.CreateControl(0, "Box", position, {200,25});
            return control;
        };
        
        if (node!=treeView->Root()) {
            EditorObject* editorObject = node->GetComponent<EditorObject>();
            if (editorObject && editorObject->editorObject) {
                GameObject* selectButton = gui.CreateControl(clone, "TextBox", {25,0}, {200-25,25});
                selectButton->GetComponent<Touchable>()->Click.Bind(this, &HierarchyWindow::Clicked, editorObject->editorObject);
                selectButton->AddComponent<Selectable>(editorObject->editorObject);
                selectButton->AddComponent<SelectedColorer>()->Selected = Colour::Blue();
                selectButton->GetComponent<Touchable>()->ClickThrough = true;
                selectButton->AddComponent<EditorObject>(node);
            }
        } else {
            rootItem = clone;
        }
        return clone;
    };
    
    spawner->OnRemove = [this] (GameObject* node, GameObject* control) {
        if (control == rootItem) {
            rootItem = 0;
        }
    };
    
    window->GetComponent<Transform>()->Position += {300,200};
}

void HierarchyWindow::Clicked(TouchData d, GameObject* object) {
    auto selectables = context->Project().GetSelectables();
    selectables->ClearSelection();
    object->GetComponent<Selectable>()->Selected = true;
    //factory->GetSelectable(object)->Selected = true;
    //object->GetComponent<Selectable>()->Selected = true;
}

void HierarchyWindow::Dropped(Pocket::DroppedData d, Pocket::GameObject *object) {

    EditorObject* source = object->GetComponent<EditorObject>();
    if (!source) {
        return;
    }
    
    for(auto& t : d.droppedTouches) {
        EditorObject* destination = t.object->GetComponent<EditorObject>();
        if (destination) {
            if (source!=destination && IsParentValid(source->gameObject, destination->gameObject)) {
                source->gameObject->Parent = destination->gameObject;
                return;
            }
        } else if (t.object == rootItem) {
            source->gameObject->Parent = 0;
        }
    }
}

bool HierarchyWindow::IsParentValid(Pocket::GameObject *object, Pocket::GameObject *possibleParent) {
    GameObject* o = possibleParent;
    while (o) {
        if (o == object) return false;
        o = o->Parent();
    }
    return true;
}