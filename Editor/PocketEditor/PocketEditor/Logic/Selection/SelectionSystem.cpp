//
//  SelectionSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "SelectionSystem.hpp"

void SelectionSystem::ObjectAdded(Pocket::GameObject *object) {
    
    GameObject* editorObject = editorWorld->CreateObject();
    
    editorObject->AddComponent<Transform>(object);
    editorObject->AddComponent<Mesh>(object);
    editorObject->AddComponent<Selectable>(factory->AddSelectable(object));
    editorObject->AddComponent<Touchable>();
    editorObject->AddComponent<EditorSelection>();
    
    SetMetaData(object, editorObject);
}

void SelectionSystem::SetEditorWorld(Pocket::GameWorld *editorWorld) {
    this->editorWorld = editorWorld;
    factory = this->editorWorld->CreateSystem<SelectableFactory>();
}

void SelectionSystem::ObjectRemoved(Pocket::GameObject *object) {
    GameObject* editorObject = (GameObject*)GetMetaData(object);
    factory->RemoveSelectable(object);
    editorObject->Remove();
}