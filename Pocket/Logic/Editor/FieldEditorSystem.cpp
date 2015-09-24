//
//  FieldEditorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "FieldEditorSystem.hpp"
#include "SerializedFieldEditors.hpp"

void FieldEditorSystem::AddedToWorld(Pocket::GameWorld &world) {
    CreateDefaultSerializedEditors();
}

void FieldEditorSystem::ObjectAdded(GameObject* object) {
    object->GetComponent<FieldEditor>()->Object.Changed += event_handler(this, &FieldEditorSystem::FieldChanged, object);
    object->GetComponent<FieldEditor>()->Field.Changed += event_handler(this, &FieldEditorSystem::FieldChanged, object);
    FieldChanged(object->GetComponent<FieldEditor>(), object);
}

void FieldEditorSystem::ObjectRemoved(GameObject* object) {
    object->GetComponent<FieldEditor>()->Object.Changed -= event_handler(this, &FieldEditorSystem::FieldChanged, object);
    object->GetComponent<FieldEditor>()->Field.Changed -= event_handler(this, &FieldEditorSystem::FieldChanged, object);
}

void FieldEditorSystem::Update(float dt) {
    for(auto object : Objects()) {
        if (object->GetComponent<FieldEditor>()->editor) {
            object->GetComponent<FieldEditor>()->editor->Update(dt);
        }
    }
}

void FieldEditorSystem::FieldChanged(FieldEditor* editor, GameObject* object) {
    if (editor->editor) {
        editor->editor->Destroy();
        delete editor->editor;
        editor->editor = 0;
    }
    if (!editor->Object()) {
        return;
    }
    auto fields = editor->Object()->GetFields();
    ISerializedField* field = fields.GetField(editor->Field);
    if (field) {
        editor->editor = field->CreateEditor(gui, object);
    }
}

