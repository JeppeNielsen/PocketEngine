//
//  EditorSizeableSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorSizeableSystem.hpp"
#include "Renderable.hpp"
#include "Mesh.hpp"
#include "Draggable.hpp"
#include "Touchable.hpp"

void EditorSizeableSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Bind(this, &EditorSizeableSystem::SelectionChanged, object);
}

void EditorSizeableSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Unbind(this, &EditorSizeableSystem::SelectionChanged, object);
    TryRemoveTransformObject(object);
}

void EditorSizeableSystem::SelectionChanged(Pocket::GameObject *object) {
    if (object->GetComponent<Selectable>()->Selected) {
        GameObject* selectionObject = root->CreateObject();
        selectionObject->AddComponent<Transform>(object);
        GameObject* dragger = selectionObject->CreateChild();
        dragger->AddComponent<Transform>();
        dragger->AddComponent<Renderable>();
        dragger->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 1, Colour::White());
        dragger->AddComponent<Draggable>();
        dragger->AddComponent<Touchable>();
        dragger->GetComponent<Transform>()->Position = object->GetComponent<Sizeable>()->Size();
        DraggerEvent e { dragger, object };
        dragger->GetComponent<Transform>()->Position.Changed.Bind(this, &EditorSizeableSystem::DraggerPositionChanged, e);
        
        transformObjects[object] = selectionObject;
    } else {
       TryRemoveTransformObject(object);
    }
}

void EditorSizeableSystem::TryRemoveTransformObject(Pocket::GameObject *object) {
    auto it = transformObjects.find(object);
    if (it!=transformObjects.end()) {
        GameObject* dragger = it->second->Children()[0];
        DraggerEvent e { dragger, object };
        dragger->GetComponent<Transform>()->Position.Changed.Unbind(this, &EditorSizeableSystem::DraggerPositionChanged, e);
        it->second->Remove();
        transformObjects.erase(it);
    }
}

void EditorSizeableSystem::DraggerPositionChanged(DraggerEvent e) {
    if (!e.dragger->GetComponent<Transform>()) return;
    Vector3 pos = e.dragger->GetComponent<Transform>()->Position;
    e.object->GetComponent<Sizeable>()->Size = pos;
}

void EditorSizeableSystem::Update(float dt) {
    for(auto it : transformObjects) {
    //    it.second->GetComponent<Transform>()->Position = it.first->GetComponent<Transform>()->Position;
    }
}
