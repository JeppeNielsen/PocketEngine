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
#include "EditorDropTarget.hpp"
#include "Cloner.hpp"

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
    
    if (old) {
        old->GameRoot.Changed.Unbind(this, &HierarchyWindow::GameSceneChanged, old);
    }
    
    if (current) {
        current->GameRoot.Changed.Bind(this, &HierarchyWindow::GameSceneChanged, current);
    }
    GameSceneChanged(current);
}

void HierarchyWindow::GameSceneChanged(OpenWorld *world) {
    GameObject* root =  world ? world->GameRoot : 0;
    context->preActions.emplace_back([this, root] () {
        treeView->Root = root;
    });
}

void HierarchyWindow::OnCreate() {
    Gui& gui = context->Gui();

    GameObject* pivot;
    GameObject* listBox = gui.CreateListbox(window, "Box", {0,0}, {200,window->GetComponent<Sizeable>()->Size().y-50}, &pivot);
    listBox->RemoveComponent<Sprite>();

    treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->ShowRoot = true;
    treeView->Root = 0;
    //treeView->SetNodeExpanded(root, true);
    treeView->Pivot = listBox;
    treeView->PredicateFunction = [] (GameObject* object) {
        if (object->Parent() && object->Parent()->GetComponent<Cloner>()) return false;
        return true;
    };
    
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    
    spawner->HasChildren = [] (GameObject* object) {
        if (object->GetComponent<Cloner>()) return false;
        return !object->Children().empty();
    };
    
    spawner->OnCreate = [&, this](VirtualTreeListSpawner::SpawnedNode& n) {
        
        GameObject* clone = gui.CreateControl(n.parent, "Box", 0, {200,n.height});
        clone->AddComponent<Droppable>()->Dropped.Bind(this, &HierarchyWindow::Dropped, n.node);
        if (n.hasChildren) {
            n.foldOutButton = gui.CreateControl(clone, "TextBox", 0, {25,25});
        }
        
        clone->GetComponent<Droppable>()->OnCreate = [&gui, this](GameObject* o, TouchData d) -> GameObject* {
            Vector3 position = o->GetComponent<Transform>()->World().TransformPosition(0);
            GameObject* control = gui.CreateControl(0, "Box", position, {200,25});
            return control;
        };
        EditorObject* editorObject = n.node->GetComponent<EditorObject>();
    
        if (editorObject && editorObject->editorObject) {
        
            GameObject* selectButton = gui.CreateControl(clone, "TextBox", {25,0}, {200-25,25});
            selectButton->GetComponent<Touchable>()->Click.Bind(this, &HierarchyWindow::Clicked, editorObject->editorObject);
            selectButton->AddComponent<Colorable>()->Color = Colour::White();
            selectButton->GetComponent<Touchable>()->ClickThrough = true;
            
            
            objectToSelectButton[editorObject->editorObject] = selectButton;
            editorObject->editorObject->GetComponent<Selectable>()->Selected.Changed.Bind(this, &::HierarchyWindow::SelectedChanged, editorObject->editorObject);
            SetNodeSelected(selectButton, editorObject->editorObject->GetComponent<Selectable>()->Selected);
            
            {
                std::stringstream str;
                str<<editorObject->gameObject->RootId();
                
                GameObject* label = gui.CreateLabel(clone, {0,0}, {60,25}, 0, str.str(), 12);
                
                label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
                label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
                label->AddComponent<Colorable>()->Color = Colour::Black();
            }
            
            GameObject* enableButton = gui.CreateControl(clone, "TextBox", {25, 0}, {25,25});
            enableButton->GetComponent<Touchable>()->Click.Bind([editorObject] (TouchData d) {
                editorObject->gameObject->Enabled = !editorObject->gameObject->Enabled;
            });
            editorObject->gameObject->Enabled.Changed.Bind(this, &::HierarchyWindow::EnabledChanged, editorObject->gameObject);
            SetNodeEnabled(enableButton, editorObject->gameObject->Enabled);
            objectToEnableButton[editorObject->gameObject] = enableButton;
        }
    };
    
    spawner->OnRemove = [this] (auto& n) {
        /*if (n.control == rootItem) {
            rootItem = 0;
        }*/
        
        EditorObject* editorObject = n.node->template GetComponent<EditorObject>();
        if (editorObject) {
        
            {
                editorObject->editorObject->GetComponent<Selectable>()->Selected.Changed.Unbind(this, &::HierarchyWindow::SelectedChanged, editorObject->editorObject);
                auto it = objectToSelectButton.find(editorObject->editorObject);
                if (it!=objectToSelectButton.end()) {
                    objectToSelectButton.erase(it);
                }
            }
            
            {
                editorObject->gameObject->Enabled.Changed.Unbind(this, &::HierarchyWindow::EnabledChanged, editorObject->gameObject);
                auto it = objectToEnableButton.find(editorObject->gameObject);
                if (it!=objectToEnableButton.end()) {
                    objectToEnableButton.erase(it);
                }
            }
        }
    };
    
    window->GetComponent<Transform>()->Position += {300,200};
}

void HierarchyWindow::SelectedChanged(Pocket::GameObject *object) {
    SetNodeSelected(objectToSelectButton[object], object->GetComponent<Selectable>()->Selected);
}

void HierarchyWindow::SetNodeSelected(Pocket::GameObject *node, bool enabled) {
    node->GetComponent<Colorable>()->Color = enabled ? Colour(0.0f, 0.0f, 1.0f, 1.0f) : Colour(1.0f, 1.0f, 1.0f, 1.0f);
}

void HierarchyWindow::EnabledChanged(Pocket::GameObject *object) {
    SetNodeEnabled(objectToEnableButton[object], object->Enabled);
}

void HierarchyWindow::SetNodeEnabled(Pocket::GameObject *node, bool enabled) {
    node->GetComponent<Colorable>()->Color = enabled ? Colour(0.5f, 0.5f, 0.5f, 0.5f) : Colour(0.5f, 0.5f, 0.5f, 0.15f);
}

void HierarchyWindow::Clicked(TouchData d, GameObject* object) {
    auto selectables = context->Project().GetSelectables();
    selectables->ClearSelection();
    if (!object->GetComponent<Selectable>()) return;
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
        EditorObject* destination = 0;//t.object->GetComponent<EditorObject>();
        for(auto it : objectToSelectButton) {
            if (it.second == t.object) {
                destination = it.first->GetComponent<EditorObject>();
                break;
            }
        }
        
        if (destination) {
            if (source!=destination && IsParentValid(source->gameObject, destination->gameObject)) {
                source->gameObject->Parent = destination->gameObject;
                return;
            }
        } else if (t.object == rootItem) {
            source->gameObject->Parent = source->gameObject->Root();
        } else {
            EditorDropTarget* target = t.object->GetComponent<EditorDropTarget>();
            if (target && target->OnDropped) {
                target->OnDropped(source);
            }
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
