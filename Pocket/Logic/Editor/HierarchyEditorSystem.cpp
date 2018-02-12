//
//  HierarchyEditorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 02/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "HierarchyEditorSystem.hpp"
#include "Selectable.hpp"
#include "SelectedColorer.hpp"
#include "Colorable.hpp"
#include "Droppable.hpp"
using namespace Pocket;

void HierarchyEditorSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<HierarchyEditor>()->Object.Changed.Bind(this, &HierarchyEditorSystem::ObjectChanged, object);
    ObjectChanged(object);
}

void HierarchyEditorSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<HierarchyEditor>()->Object.Changed.Unbind(this, &HierarchyEditorSystem::ObjectChanged, object);
    object->GetComponent<HierarchyEditor>()->Object = 0;
}

void HierarchyEditorSystem::Update(float dt) {
    for (auto o : Objects()) {
        HierarchyEditor* editor = o->GetComponent<HierarchyEditor>();
        if (editor->Object && editor->prevChildrenCount!=editor->Object()->Hierarchy().Children().size()) {
            ObjectChanged(o);
        }
    }
}

void HierarchyEditorSystem::ObjectChanged(GameObject* object) {
    HierarchyEditor *editor = object->GetComponent<HierarchyEditor>();
    if (object->IsRemoved()) return;
    editor->prevChildrenCount = editor->Object()->Hierarchy().Children().size();
    for (auto child : object->Hierarchy().Children()) {
        child->Remove();
    }
    if (!(editor->Object)) {
        return;
    }
    
    int depth = CountDepth(editor->Object);
    
    std::stringstream s;
    s<<(editor->Object)<<"  " << depth;
    
    ObjectCollection children = editor->Object()->Hierarchy().Children();
    
    std::sort(children.begin(), children.end(), [] (GameObject* a, GameObject* b) {
        return a->Hierarchy().Order()<b->Hierarchy().Order();
    });
    
    for (auto child : children) {
        if (!child->GetComponent<Atlas>()) continue;
        
        GameObject* childObject = object->CreateChild();
        childObject->AddComponent<Transform>();
        childObject->AddComponent<Sizeable>()->Size = {200,10};
        //childObject->AddComponent<Layoutable>()->ChildLayouting = Layoutable::ChildLayouting::VerticalStackedFit;
        //childObject->GetComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
        childObject->AddComponent<HierarchyEditor>()->Object = child;
    }
    
    GameObject* gameObjectName = gui->CreateTextBox(object, "Box", {20.0f * depth,0}, {200-depth*20.0f,20}, 0, s.str(), 14);
    gameObjectName->Hierarchy().Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    //gameObjectName->AddComponent<Layoutable>();
    gameObjectName->AddComponent<Selectable>(editor->Object);
    gameObjectName->GetComponent<Colorable>()->Color = Colour::White(0.5f);
    gameObjectName->AddComponent<SelectedColorer>()->Selected = Colour(0.5f, 0.5f, 0.5f, 1.0f);
    gameObjectName->AddComponent<Draggable>();
    gameObjectName->AddComponent<Droppable>()->Dropped.Bind(this, &HierarchyEditorSystem::OnDropped, editor->Object());
}

int HierarchyEditorSystem::CountDepth(Pocket::GameObject *object) {
    int depth = -1;
    while (object) {
        depth++;
        object = object->Hierarchy().Parent();
    }
    return depth;
}

void HierarchyEditorSystem::OnDropped(Pocket::DroppedData d, Pocket::GameObject *editorObject) {
    for (TouchData& touchData : d.droppedTouches) {
        if (!touchData.object->Hierarchy().Parent()) continue;
        HierarchyEditor* editor = touchData.object->Hierarchy().Parent()->GetComponent<HierarchyEditor>();
        if (!editor) continue;
        GameObject* newParent = editor->Object;
        
        if (!IsParentLegal(newParent, editorObject)) {
            return;
        }
        Transform* transform = editorObject->GetComponent<Transform>();
        Vector3 worldPosition = transform->World().TransformPosition(0);
        editorObject->Hierarchy().Parent = newParent;
        Vector3 localPosition = transform->World().Invert().TransformPosition(worldPosition);
        transform->Position = transform->Local().TransformPosition(localPosition);
        break;
    }
}

bool HierarchyEditorSystem::IsParentLegal(Pocket::GameObject *parent, Pocket::GameObject *ancestor) {
    while (parent) {
        if (parent == ancestor) return false;
        parent = parent->Hierarchy().Parent();
    }
    return true;
}
