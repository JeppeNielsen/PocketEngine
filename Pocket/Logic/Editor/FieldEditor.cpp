//
//  FieldEditor.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "FieldEditor.hpp"

using namespace Pocket;

FieldEditor::FieldEditor() :editor(0) {
    editor = 0;
    Field = "";
}

void FieldEditor::operator=(const FieldEditor& other) {
    if (editor) {
        editor->Destroy();
    }
    editor = 0;
    Field = "";
}

void FieldEditor::SetType(const Pocket::TypeInfo& type) {
    Type = type;
    TypeChanged();
}