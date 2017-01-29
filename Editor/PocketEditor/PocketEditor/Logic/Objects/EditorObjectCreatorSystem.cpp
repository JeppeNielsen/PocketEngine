//
//  EditorObjectCreatorSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 14/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorObjectCreatorSystem.hpp"
#include "Touchable.hpp"
#include "Sizeable.hpp"

void EditorObjectCreatorSystem::Initialize() {
    root->CreateSystem<ComponentSystem<Transform>>()->creatorSystem = this;
    root->CreateSystem<ComponentSystem<Mesh>>()->creatorSystem = this;
    root->CreateSystem<ComponentSystem<Sizeable>>()->creatorSystem = this;
    Order = -1000;
}

void EditorObjectCreatorSystem::ObjectAdded(Pocket::GameObject *object) {
    
    GameObject* editorObject = editorRoot->CreateObject();
    editorObject->AddComponent<Selectable>();
    //editorObject->AddComponent<Touchable>()->ClickThrough = true;
    
    object->GetComponent<EditorObject>()->gameObject = object;
    object->GetComponent<EditorObject>()->editorObject = editorObject;
    
    editorObject->AddComponent<EditorObject>(object);
    SetMetaData(object, editorObject);
}

void EditorObjectCreatorSystem::ObjectRemoved(Pocket::GameObject *object) {
    GameObject* editorObject = (GameObject*)GetMetaData(object);
    editorObject->Remove();
}
