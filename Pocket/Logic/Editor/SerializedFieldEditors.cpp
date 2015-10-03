//
//  SerializedFieldEditors.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SerializedFieldEditors.hpp"

//-------- float ---------
void SerializedFieldEditorFloat::Initialize(Gui* context, GameObject* parent) {
    Vector2 size = parent->GetComponent<Sizeable>()->Size;
    textBox = context->CreateTextBox(parent, "TextBox", 0, size, 0, "", 15.0f);
    textBox->GetComponent<Touchable>()->ClickThrough = true;
    textBox->Children()[0]->GetComponent<TextBox>()->Active.Changed += event_handler(this, &SerializedFieldEditorFloat::TextChanged);
    textBox->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    prev = (*field) - 1;
}

void SerializedFieldEditorFloat::Destroy() {
    textBox->Children()[0]->GetComponent<TextBox>()->Active.Changed -= event_handler(this, &SerializedFieldEditorFloat::TextChanged);
    textBox->Remove();
}

void SerializedFieldEditorFloat::TextChanged(TextBox* textBox) {
    if (textBox->Active) return;
    float value = (float)atof(textBox->Text().c_str());
    (*field) = value;
    //prev = (*field);
}

void SerializedFieldEditorFloat::Update(float dt) {
    bool changed = prev!=(*field);
    if (changed && !textBox->Children()[0]->GetComponent<TextBox>()->Active()) {
        std::stringstream s;
        s<<(*field);
        textBox->Children()[0]->GetComponent<TextBox>()->Text = s.str();
    }
    prev = (*field);
}

//-------- Vector2 ---------
void SerializedFieldEditorVector2::Initialize(Gui* context, GameObject* parent) {
    Vector2 size = parent->GetComponent<Sizeable>()->Size;
    size.x /= 2.0f;
    for (int i=0; i<2; ++i) {
        textBox[i] = context->CreateTextBox(parent, "TextBox", {i*size.x,0}, size, 0, "", 15.0f);
        textBox[i]->GetComponent<Touchable>()->ClickThrough = true;
        textBox[i]->Children()[0]->GetComponent<TextBox>()->Active.Changed += event_handler(this, &SerializedFieldEditorVector2::TextChanged,  textBox[i]);
        textBox[i]->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    }
    prev = (*field) - 1;
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
        textBox[i] = context->CreateTextBox(parent, "TextBox", {i*size.x,0}, size, 0, "", 15.0f);
        textBox[i]->GetComponent<Touchable>()->ClickThrough = true;
        textBox[i]->Children()[0]->GetComponent<TextBox>()->Active.Changed += event_handler(this, &SerializedFieldEditorVector3::TextChanged,  textBox[i]);
        textBox[i]->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    }
    prev = (*field) - 1;
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

//-------- std::string ---------
void SerializedFieldEditorString::Initialize(Gui* context, GameObject* parent) {
    Vector2 size = parent->GetComponent<Sizeable>()->Size;

    textBox = context->CreateTextBox(parent, "TextBox", 0, size, 0, "", 15.0f);
    textBox->GetComponent<Touchable>()->ClickThrough = true;
    textBox->Children()[0]->GetComponent<TextBox>()->Active.Changed += event_handler(this, &SerializedFieldEditorString::TextChanged);
    textBox->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
}

void SerializedFieldEditorString::Destroy() {
    textBox->Children()[0]->GetComponent<TextBox>()->Active.Changed -= event_handler(this, &SerializedFieldEditorString::TextChanged);
    textBox->Remove();
}

void SerializedFieldEditorString::TextChanged(TextBox* textBox) {
    if (textBox->Active) return;
    (*field) = textBox->Text();
}

void SerializedFieldEditorString::Update(float dt) {
    bool changed = prev!=(*field);
    if (changed && !textBox->Children()[0]->GetComponent<TextBox>()->Active()) {
        textBox->Children()[0]->GetComponent<TextBox>()->Text = (*field);
    }
    prev = (*field);
}

//-------- bool ---------
void SerializedFieldEditorBool::Initialize(Gui* context, GameObject* parent) {
    Vector2 size = parent->GetComponent<Sizeable>()->Size;

    box = context->CreateControl(parent, "TextBox", 0, size);
    box->GetComponent<Touchable>()->ClickThrough = true;
    box->GetComponent<Touchable>()->Click += event_handler(this, &SerializedFieldEditorBool::Clicked);
    tick = context->CreateControl(box, "TextBox", size*0.1f, size*0.8f);
    tick->RemoveComponent<Touchable>();
    tick->GetComponent<Colorable>()->Color = Colour::Black();
    prev = !(*field);
    Update(0);
}

void SerializedFieldEditorBool::Destroy() {
   box->GetComponent<Touchable>()->Click -= event_handler(this, &SerializedFieldEditorBool::Clicked);
   box->Remove();
}

void SerializedFieldEditorBool::Clicked(TouchData touch) {
    (*field) = !(*field);
}

void SerializedFieldEditorBool::Update(float dt) {
    bool changed = prev!=(*field);
    if (changed) {
        tick->EnableComponent<Material>(*field);
    }
    prev = (*field);
}


//-------- Quaternion ---------
void SerializedFieldEditorQuaternion::Initialize(Gui* context, GameObject* parent) {
    Vector2 size = parent->GetComponent<Sizeable>()->Size;
    size.x /= 3.0f;
    for (int i=0; i<3; ++i) {
        textBox[i] = context->CreateTextBox(parent, "TextBox", {i*size.x,0}, size, 0, "", 15.0f);
        textBox[i]->GetComponent<Touchable>()->ClickThrough = true;
        textBox[i]->Children()[0]->GetComponent<TextBox>()->Active.Changed += event_handler(this, &SerializedFieldEditorQuaternion::TextChanged,  textBox[i]);
        textBox[i]->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    }
    prev = (*field).ToEuler()-1;
}

void SerializedFieldEditorQuaternion::Destroy() {
    for (int i=0; i<3; ++i) {
        textBox[i]->Children()[0]->GetComponent<TextBox>()->Active.Changed -= event_handler(this, &SerializedFieldEditorQuaternion::TextChanged, textBox[i]);
        textBox[i]->Remove();
    }
}

void SerializedFieldEditorQuaternion::TextChanged(TextBox* textBox, GameObject* object) {
    if (textBox->Active) return;
    float value = (float)atof(textBox->Text().c_str());
    Vector3 euler = (*field).ToEuler();
    euler *= MathHelper::RadToDeg;
    if (object == this->textBox[0]) {
        euler.x = value;
    } else if (object == this->textBox[1]) {
        euler.y = value;
    } else {
        euler.z = value;
    }
    euler *= MathHelper::DegToRad;
    (*field) = Quaternion(euler);
    //prev = (*field);
}

void SerializedFieldEditorQuaternion::Update(float dt) {
    Vector3 euler = (*field).ToEuler();
    euler *= MathHelper::RadToDeg;
    for (int i=0; i<3; ++i) {
        bool changed = prev[i]!=(*field)[i];
        if (changed && !textBox[i]->Children()[0]->GetComponent<TextBox>()->Active()) {
            std::stringstream s;
            s<<euler[i];
            textBox[i]->Children()[0]->GetComponent<TextBox>()->Text = s.str();
        }
    }
    prev = euler;
}


void Pocket::CreateDefaultSerializedEditors() {
    SerializedField<float>::Editor = [] () { return new SerializedFieldEditorFloat(); };
    SerializedField<Vector2>::Editor = [] () { return new SerializedFieldEditorVector2(); };
    SerializedField<Vector3>::Editor = [] () { return new SerializedFieldEditorVector3(); };
    SerializedField<std::string>::Editor = [] () { return new SerializedFieldEditorString(); };
    SerializedField<bool>::Editor = [] () { return new SerializedFieldEditorBool(); };
    SerializedField<Quaternion>::Editor = [] () { return new SerializedFieldEditorQuaternion(); };
}
