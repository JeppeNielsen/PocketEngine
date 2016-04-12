//
//  EditorTransformCreationSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorTransformCreationSystem.hpp"
#include "Touchable.hpp"

void EditorTransformCreationSystem::ObjectAdded(Pocket::GameObject *object) {
    GameObject* editorObject = editorWorld->CreateObject();
    
    editorObject->AddComponent<Transform>(object);
    editorObject->AddComponent<Mesh>(object);
    editorObject->AddComponent<Selectable>(factory->AddSelectable(object));
    editorObject->AddComponent<Touchable>();
    editorObject->AddComponent<EditorTransform>();
    
    SetMetaData(object, editorObject);
}

void EditorTransformCreationSystem::ObjectRemoved(Pocket::GameObject *object) {
    GameObject* editorObject = (GameObject*)GetMetaData(object);
    factory->RemoveSelectable(object);
    editorObject->Remove();
}

void EditorTransformCreationSystem::SetEditorWorld(Pocket::GameWorld *editorWorld) {
    this->editorWorld = editorWorld;
    factory = this->editorWorld->CreateSystem<SelectableFactory>();
}