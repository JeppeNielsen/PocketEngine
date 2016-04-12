//
//  EditorSelectionSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorSelectionSystem.hpp"


void EditorSelectionSystem::Initialize(Pocket::GameWorld *world) {
    this->world = world;
}

void EditorSelectionSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Bind(this, &EditorSelectionSystem::SelectionChanged, object);
}

void EditorSelectionSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Unbind(this, &EditorSelectionSystem::SelectionChanged, object);
    
    if (object->GetComponent<EditorSelection>()->selectionObject) {
        object->GetComponent<EditorSelection>()->selectionObject->Remove();
        object->GetComponent<EditorSelection>()->selectionObject = 0;
    }
}

void EditorSelectionSystem::SelectionChanged(Pocket::GameObject *object) {
    if (object->GetComponent<Selectable>()->Selected) {
        auto editorSelection = object->GetComponent<EditorSelection>();
        GameObject* selectionObject = world->CreateObject();
        selectionObject->AddComponent<Transform>(object);
        selectionObject->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1.05f);
        selectionObject->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour(0,0,1.0f, 0.5f));
        selectionObject->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
        editorSelection->selectionObject = selectionObject;
    } else {
        if (object->GetComponent<EditorSelection>()->selectionObject) {
            object->GetComponent<EditorSelection>()->selectionObject->Remove();
            object->GetComponent<EditorSelection>()->selectionObject = 0;
        }
    }
}