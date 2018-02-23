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
#include "GameObjectFieldEditor.hpp"
#include "Cloner.hpp"

std::string HierarchyWindow::Name() { return "Hierarchy"; }

void HierarchyWindow::OnInitialize() {
    GameObject& guiRoot = context->GuiRoot();
    /*guiRoot.CreateSystem<VirtualTreeListSystem>();
    guiRoot.CreateSystem<VirtualTreeListSpawnerSystem>();
    guiRoot.CreateSystem<SelectedColorerSystem>();
    guiRoot.CreateSystem<DraggableSystem>();
    */
    editorGui = guiRoot.CreateSystem<EditorGui>();
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
    GameObject* root =  world ? world->GameRoot : nullptr;
    context->preActions.emplace_back([this, root] () {
        treeView->Root = root;
    });
}

void HierarchyWindow::OnCreate() {
    
    GameObject* treeViewGo = editorGui->CreateTreeList(window, nullptr,
        nullptr,
        [] (GameObject* object) {
            if (object->IsRemoved()) return false;
            Hierarchy* hier = &object->Hierarchy();
            if (hier->Parent() && hier->Parent()->GetComponent<Cloner>()) return false;
            return true;
        }, [] (GameObject* object) {
            if (object->GetComponent<Cloner>()) return false;
            return !object->Hierarchy().Children().empty();
        },
        [this] (auto& n, GameObject* button, std::string& text) {
            
            EditorObject* editorObject = n.node->template GetComponent<EditorObject>();
    
            if (editorObject && editorObject->editorObject) {
            
                button->GetComponent<Touchable>()->Click.Bind(this, &HierarchyWindow::Clicked, editorObject->editorObject);
                
                objectToSelectButton[editorObject->editorObject] = button;
                editorObject->editorObject->GetComponent<Selectable>()->Selected.Changed.Bind(this, &::HierarchyWindow::SelectedChanged, editorObject->editorObject);
                SetNodeSelected(button, editorObject->editorObject->GetComponent<Selectable>()->Selected);
                
                std::stringstream str;
                str<<editorObject->gameObject->RootId();
                    
                text = str.str();
                
                Gui& gui = context->Gui();
                
                GameObject* flexibleSpace = gui.CreatePivot(button);
                gui.AddLayouter(flexibleSpace, {0,18}, {2000,18}, {2000,18});
                
                GameObject* enableButton = gui.CreateControl(button, "CheckBox_on", {25, 0}, {25,25});
                gui.AddLayouter(enableButton, 12, 12, 12);
                enableButton->GetComponent<Touchable>()->Click.Bind([editorObject] (TouchData d) {
                    editorObject->gameObject->Hierarchy().Enabled = !editorObject->gameObject->Hierarchy().Enabled;
                });
                editorObject->gameObject->Hierarchy().Enabled.Changed.Bind(this, &::HierarchyWindow::EnabledChanged, editorObject->gameObject);
                SetNodeEnabled(enableButton, editorObject->gameObject->Hierarchy().Enabled);
                objectToEnableButton[editorObject->gameObject] = enableButton;
                
                button->AddComponent<Droppable>()->Dropped.Bind(this, &HierarchyWindow::Dropped, n.node);

                button->GetComponent<Droppable>()->OnCreate = [&gui, this](GameObject* o, TouchData d) -> GameObject* {
                    Vector3 position = o->GetComponent<Transform>()->World().TransformPosition(0);
                    GameObject* control = gui.CreateControl(0, "Box", position, {200,25});
                    return control;
                };
            }
        },
        [this] (auto& n, GameObject* button) {
        
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
                    editorObject->gameObject->Hierarchy().Enabled.Changed.Unbind(this, &::HierarchyWindow::EnabledChanged, editorObject->gameObject);
                    auto it = objectToEnableButton.find(editorObject->gameObject);
                    if (it!=objectToEnableButton.end()) {
                        objectToEnableButton.erase(it);
                    }
                }
            }
        },
        true
    );
    treeView = treeViewGo->GetComponent<VirtualTreeList>();
}

void HierarchyWindow::SelectedChanged(Pocket::GameObject *object) {
    SetNodeSelected(objectToSelectButton[object], object->GetComponent<Selectable>()->Selected);
}

void HierarchyWindow::SetNodeSelected(Pocket::GameObject *node, bool enabled) {
    node->GetComponent<Selectable>()->Selected = enabled;
}

void HierarchyWindow::EnabledChanged(Pocket::GameObject *object) {
    SetNodeEnabled(objectToEnableButton[object], object->Hierarchy().Enabled);
}

void HierarchyWindow::SetNodeEnabled(Pocket::GameObject *node, bool enabled) {
    if (node->HasComponent<Sprite>()) {
        node->GetComponent<Sprite>()->SpriteName = enabled ? "CheckBox_on" : "CheckBox_off";
    }
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
                source->gameObject->Hierarchy().Parent = destination->gameObject;
                return;
            }
        } else if (t.object == rootItem) {
            source->gameObject->Hierarchy().Parent = source->gameObject->Root();
        } else {
        
            GameObjectFieldEditor* goField = t.object->GetComponent<GameObjectFieldEditor>();
            if (goField && goField->SetObject) {
                goField->SetObject(source->gameObject);
            }
        }
    }
}

bool HierarchyWindow::IsParentValid(Pocket::GameObject *object, Pocket::GameObject *possibleParent) {
    GameObject* o = possibleParent;
    while (o) {
        if (o == object) return false;
        o = o->Hierarchy().Parent();
    }
    return true;
}
