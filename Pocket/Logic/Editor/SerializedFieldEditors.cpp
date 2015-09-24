//
//  SerializedFieldEditors.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SerializedFieldEditors.hpp"

//-------- Vector2 ---------
void SerializedFieldEditorVector2::Initialize(Gui* context, GameObject* parent) {
    Vector2 size = parent->GetComponent<Sizeable>()->Size;
    size.x /= 2.0f;
    for (int i=0; i<2; ++i) {
        textBox[i] = context->CreateTextBox(parent, "Box", {i*size.x,0}, size, 0, "", 15.0f);
        textBox[i]->Children()[0]->GetComponent<TextBox>()->Active.Changed += event_handler(this, &SerializedFieldEditorVector2::TextChanged,  textBox[i]);
        textBox[i]->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    }
}

void SerializedFieldEditorVector2::Destroy() {
    for (int i=0; i<2; ++i) {
        textBox[i]->Children()[0]->GetComponent<TextBox>()->Active.Changed -= event_handler(this, &SerializedFieldEditorVector2::TextChanged, textBox[i]);
        textBox[i]->Remove();
    }
}

void SerializedFieldEditorVector2::TextChanged(TextBox* textBox, GameObject* object) {
    if (textBox->Active) return;
    float value = (float)atof(textBox->Text().c_str());
    if (object == this->textBox[0]) {
        (*field).x = value;
    } else if (object == this->textBox[1]) {
        (*field).y = value;
    }
    //prev = (*field);
}

void SerializedFieldEditorVector2::Update(float dt) {
    for (int i=0; i<2; ++i) {
        bool changed = prev[i]!=(*field)[i];
        if (changed && !textBox[i]->Children()[0]->GetComponent<TextBox>()->Active()) {
            std::stringstream s;
            s<<(*field)[i];
            textBox[i]->Children()[0]->GetComponent<TextBox>()->Text = s.str();
        }
    }
    prev = (*field);
}


//-------- Vector3 ---------
void SerializedFieldEditorVector3::Initialize(Gui* context, GameObject* parent) {
    Vector2 size = parent->GetComponent<Sizeable>()->Size;
    size.x /= 3.0f;
    for (int i=0; i<3; ++i) {
        textBox[i] = context->CreateTextBox(parent, "Box", {i*size.x,0}, size, 0, "", 15.0f);
        textBox[i]->Children()[0]->GetComponent<TextBox>()->Active.Changed += event_handler(this, &SerializedFieldEditorVector3::TextChanged,  textBox[i]);
        textBox[i]->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    }
}

void SerializedFieldEditorVector3::Destroy() {
    for (int i=0; i<3; ++i) {
        textBox[i]->Children()[0]->GetComponent<TextBox>()->Active.Changed -= event_handler(this, &SerializedFieldEditorVector3::TextChanged, textBox[i]);
        textBox[i]->Remove();
    }
}

void SerializedFieldEditorVector3::TextChanged(TextBox* textBox, GameObject* object) {
    if (textBox->Active) return;
    float value = (float)atof(textBox->Text().c_str());
    if (object == this->textBox[0]) {
        (*field).x = value;
    } else if (object == this->textBox[1]) {
        (*field).y = value;
    } else {
        (*field).z = value;
    }
    //prev = (*field);
}

void SerializedFieldEditorVector3::Update(float dt) {
    for (int i=0; i<3; ++i) {
        bool changed = prev[i]!=(*field)[i];
        if (changed && !textBox[i]->Children()[0]->GetComponent<TextBox>()->Active()) {
            std::stringstream s;
            s<<(*field)[i];
            textBox[i]->Children()[0]->GetComponent<TextBox>()->Text = s.str();
        }
    }
    prev = (*field);
}

void Pocket::CreateDefaultSerializedEditors() {
    SerializedField<Vector3>::Editor = [] () { return new SerializedFieldEditorVector3(); };
    SerializedField<Vector2>::Editor = [] () { return new SerializedFieldEditorVector2(); };
}



