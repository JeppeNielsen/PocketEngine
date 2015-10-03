//
//  HierarchyEditorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 02/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "HierarchyEditorSystem.hpp"
#include "Layoutable.hpp"
#include "Selectable.hpp"
#include "SelectedColorer.hpp"
using namespace Pocket;

void HierarchyEditorSystem::ObjectAdded(GameObject *object) {
    //object->GetComponent<HierarchyEditor>()->Object.Changed += event_handler(this, &HierarchyEditorSystem::ObjectChanged, object);
    ObjectChanged(object->GetComponent<HierarchyEditor>(), object);
}

void HierarchyEditorSystem::ObjectRemoved(GameObject *object) {
    //object->GetComponent<HierarchyEditor>()->Object.Changed -= event_handler(this, &HierarchyEditorSystem::ObjectChanged, object);
    object->GetComponent<HierarchyEditor>()->Object = 0;
}

void HierarchyEditorSystem::Update(float dt) {
    for (auto o : Objects()) {
        HierarchyEditor* editor = o->GetComponent<HierarchyEditor>();
        if (editor->Object && editor->prevChildrenCount!=editor->Object->Children().size()) {
            ObjectChanged(editor, o);
        }
    }
}

void HierarchyEditorSystem::ObjectChanged(HierarchyEditor *editor, GameObject* object) {
    if (object->IsRemoved()) return;
    editor->prevChildrenCount = editor->Object->Children().size();
    for (auto child : object->Children()) {
        child->Remove();
    }
    if (!(editor->Object)) {
        return;
    }
    
    int depth = CountDepth(editor->Object);
    
    std::stringstream s;
    s<<(editor->Object)<<"  " << depth;
    
    ObjectCollection children = editor->Object->Children();
    
    std::sort(children.begin(), children.end(), [] (GameObject* a, GameObject* b) {
        return a->Order()<b->Order();
    });
    
    for (auto child : children) {
        if (!child->GetComponent<Atlas>()) continue;
        
        GameObject* childObject = World()->CreateObject();
        childObject->AddComponent<Transform>();
        childObject->AddComponent<Sizeable>()->Size = {200,10};
        childObject->AddComponent<Layoutable>()->ChildLayouting = Layoutable::ChildLayouting::VerticalStackedFit;
        childObject->GetComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
        childObject->AddComponent<HierarchyEditor>()->Object = child;
        childObject->Parent = object;
    }
    
    GameObject* gameObjectName = gui->CreateTextBox(object, "Box", {20.0f * depth,0}, {200-depth*20.0f,20}, 0, s.str(), 14);
    gameObjectName->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    gameObjectName->AddComponent<Layoutable>();
    gameObjectName->AddComponent<Selectable>(editor->Object);
    gameObjectName->GetComponent<Colorable>()->Color = Colour::White(0.5f);
    gameObjectName->AddComponent<SelectedColorer>()->Selected = Colour(0.5f, 0.5f, 0.5f, 1.0f);
    gameObjectName->AddComponent<Draggable>();
    
}

int HierarchyEditorSystem::CountDepth(Pocket::GameObject *object) {
    int depth = -1;
    while (object) {
        depth++;
        object = object->Parent;
    }
    return depth;
}