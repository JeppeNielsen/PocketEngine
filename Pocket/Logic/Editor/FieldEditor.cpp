//
//  FieldEditor.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "FieldEditor.hpp"

using namespace Pocket;

FieldEditor::FieldEditor() : Object(this), Field(this), editor(0) {}

 void FieldEditor::Reset() {
    if (editor) {
        editor->Destroy();
    }
    editor = 0;
    Object = 0;
    Field = "";
}