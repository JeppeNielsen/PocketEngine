//
//  FieldEditorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "FieldEditorSystem.hpp"
#include "SerializedFieldEditors.hpp"

void FieldEditorSystem::Initialize(Pocket::GameWorld* world) {
    CreateDefaultSerializedEditors();
}

void FieldEditorSystem::ObjectAdded(GameObject* object) {
    FieldEditor* editor = object->GetComponent<FieldEditor>();
    editor->Object.Changed.Bind(this, &FieldEditorSystem::FieldChanged, object);
    editor->Field.Changed.Bind(this, &FieldEditorSystem::FieldChanged,object);
    FieldChanged(object);
}

void FieldEditorSystem::ObjectRemoved(GameObject* object) {
    FieldEditor* editor = object->GetComponent<FieldEditor>();
    editor->Object.Changed.Unbind(this, &FieldEditorSystem::FieldChanged, object);
    editor->Field.Changed.Unbind(this, &FieldEditorSystem::FieldChanged, object);
}

void FieldEditorSystem::Update(float dt) {
    for(auto object : Objects()) {
        if (object->GetComponent<FieldEditor>()->editor) {
            object->GetComponent<FieldEditor>()->editor->Update(dt);
        }
    }
}

void FieldEditorSystem::FieldChanged(GameObject* object) {
    FieldEditor* editor = object->GetComponent<FieldEditor>();
    if (editor->editor) {
        editor->editor->Destroy();
        delete editor->editor;
        editor->editor = 0;
    }
    if (!editor->Object()) {
        return;
    }
    auto typeInfo = editor->Object();
    IFieldInfo* field = typeInfo->GetField(editor->Field);
    if (field) {
        editor->editor = field->CreateEditor(gui, object);
    }
}

