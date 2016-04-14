//
//  EditorMeshSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorMeshSystem.hpp"

void EditorMeshSystem::Initialize(Pocket::GameWorld *world) {
    this->world = world;
}

void EditorMeshSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Bind(this, &EditorMeshSystem::SelectionChanged, object);
}

void EditorMeshSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Unbind(this, &EditorMeshSystem::SelectionChanged, object);
    TryRemoveTransformObject(object);
}

void EditorMeshSystem::SelectionChanged(Pocket::GameObject *object) {
    if (object->GetComponent<Selectable>()->Selected) {
        GameObject* selectionObject = world->CreateObject();
        selectionObject->AddComponent<Transform>(object);
        selectionObject->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1.05f);
        selectionObject->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour(0,0,1.0f, 0.5f));
        selectionObject->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
        transformObjects[object] = selectionObject;
    } else {
       TryRemoveTransformObject(object);
    }
}

void EditorMeshSystem::TryRemoveTransformObject(Pocket::GameObject *object) {
    auto it = transformObjects.find(object);
    if (it!=transformObjects.end()) {
        it->second->Remove();
        transformObjects.erase(it);
    }
}