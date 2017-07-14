//
//  FieldEditorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "FieldEditorSystem.hpp"

using namespace Pocket;

void FieldEditorSystem::Initialize() {

}

void FieldEditorSystem::ObjectAdded(GameObject* object) {
    FieldEditor* editor = object->GetComponent<FieldEditor>();
    editor->TypeChanged.Bind(this, &FieldEditorSystem::FieldChanged, object);
    editor->Field.Changed.Bind(this, &FieldEditorSystem::FieldChanged,object);
    FieldChanged(object);
}

void FieldEditorSystem::ObjectRemoved(GameObject* object) {
    FieldEditor* editor = object->GetComponent<FieldEditor>();
    editor->TypeChanged.Unbind(this, &FieldEditorSystem::FieldChanged, object);
    editor->Field.Changed.Unbind(this, &FieldEditorSystem::FieldChanged, object);
    if (editor->editor) {
        editor->editor->Destroy();
        delete editor->editor;
        editor->editor = 0;
    }
}

void FieldEditorSystem::Update(float dt) {
    for(auto object : Objects()) {
        FieldEditor* fieldEditor = object->GetComponent<FieldEditor>();
        if (fieldEditor->editor) {
            fieldEditor->editor->Update(dt);
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
    if (editor->Type.fields.size() == 0) {
        return;
    }
    auto field = editor->Type.GetField(editor->Field);
    if (field) {
        editor->editor = field->CreateEditor();
        editor->editor->Create(gui, object);
    }
}

