//
//  GameObjectEditorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GameObjectEditorSystem.hpp"
#include "FieldEditor.hpp"
using namespace Pocket;

void GameObjectEditorSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<GameObjectEditor>()->Object.Changed += event_handler(this, &GameObjectEditorSystem::ObjectChanged, object);
    ObjectChanged(object->GetComponent<GameObjectEditor>(), object);
}

void GameObjectEditorSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<GameObjectEditor>()->Object.Changed -= event_handler(this, &GameObjectEditorSystem::ObjectChanged, object);
}

void GameObjectEditorSystem::ObjectChanged(GameObjectEditor *editor, GameObject* object) {
    for (auto child : object->Children()) {
        child->Remove();
    }
    if (!editor->Object()) return;
    
    SerializableCollection components = editor->Object()->SerializableComponents();
    
    int numberOfEditors = 0;
    for(ISerializable* component : components) {
        auto fields = component->GetFields();
        for (auto field : fields.fields) {
            if (field->HasEditor()) numberOfEditors++;
        }
    }
            
    
    Vector2 size = object->GetComponent<Sizeable>()->Size;
    size.y /= (int)numberOfEditors;
    Vector2 pos = {0,0};
    
    for(ISerializable* component : components) {
        auto fields = component->GetFields();
        for (auto field : fields.fields) {
            if (!field->HasEditor()) continue;
            GameObject* componentChild = World()->CreateObject();
            componentChild->Parent = object;
            componentChild->AddComponent<Transform>()->Position = pos;
            componentChild->AddComponent<Sizeable>()->Size = size;
            auto fieldEditor = componentChild->AddComponent<FieldEditor>();
            fieldEditor->Object = component;
            fieldEditor->Field = field->name;
            pos.y += size.y;
        }
    }
}