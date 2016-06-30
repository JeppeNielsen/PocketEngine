//
//  EditorObjectCreatorSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 14/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorObjectCreatorSystem.hpp"
#include "Touchable.hpp"

void EditorObjectCreatorSystem::Initialize() {
    world->CreateSystem<ComponentSystem<Transform>>()->creatorSystem = this;
    world->CreateSystem<ComponentSystem<Mesh>>()->creatorSystem = this;
}

void EditorObjectCreatorSystem::ObjectAdded(Pocket::GameObject *object) {
    
    GameObject* editorObject = editorWorld->CreateObject();
    editorObject->Parent() = gameRoot;
    editorObject->AddComponent<Selectable>();
    editorObject->AddComponent<Touchable>();
    
    object->GetComponent<EditorObject>()->gameObject = object;
    object->GetComponent<EditorObject>()->editorObject = editorObject;
    
    editorObject->AddComponent<EditorObject>(object);
    
    SetMetaData(object, editorObject);
}

void EditorObjectCreatorSystem::ObjectRemoved(Pocket::GameObject *object) {
    GameObject* editorObject = (GameObject*)GetMetaData(object);
    editorObject->Remove();
}