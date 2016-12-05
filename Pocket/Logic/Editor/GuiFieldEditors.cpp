//
//  FieldInfoEditors.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GuiFieldEditors.hpp"

using namespace Pocket;

static Vector2 offset = {0,0};

template<typename T, int Size>
struct FieldInfoEditorTextboxes : public GuiFieldEditor {

    void SetField(void* field) override {
        this->field = static_cast<T*>(field);
    }

    void Initialize(Gui* gui, GameObject* parent) override {
        GameObject* control = gui->CreateControl(parent);
        control->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Horizontal;
        
        for(int i=0; i<Size; ++i) {
            GameObject* textBox = gui->CreateTextBox(control, "TextBox", 0, 0, 0, "", 15.0f);
            textBox->GetComponent<Touchable>()->ClickThrough = true;
            textBox->Children()[0]->GetComponent<TextBox>()->Active.Changed.Bind(this, &FieldInfoEditorTextboxes<T, Size>::TextChanged, textBox);
            textBox->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
            textBox->AddComponent<Layouter>()->Min = {20, 20};
            textBox->GetComponent<Layouter>()->Desired = {100, 20};
            textBox->GetComponent<Layouter>()->Max = {5000, 20};
            
            textBoxes[i] = textBox;
        }
        prev = (*this->field);
        forceUpdate = true;
    }
    
    void Destroy() override {
        for(int i=0; i<Size; ++i) {
             textBoxes[i]->Children()[0]->template GetComponent<TextBox>()->Active.Changed.Unbind(this, &FieldInfoEditorTextboxes<T, Size>::TextChanged, textBoxes[i]);
             textBoxes[i]->Remove();
        }
    }
    
    void TextChanged(GameObject* textBoxGO) {
        TextBox* textBox = textBoxGO->Children()[0]->GetComponent<TextBox>();
        if (textBox->Active) return;
        
        for(int i=0; i<Size; ++i) {
             if (textBoxes[i] == textBoxGO) {
                TextboxChanged(i, textBox->Text);
             }
        }
    }
    
    void Update(float dt) override {
        bool changed = prev!=(*this->field) || forceUpdate;
        if (changed) {
            forceUpdate = false;
            for(int i=0; i<Size; ++i) {
                if (!textBoxes[i]->Children()[0]->template GetComponent<TextBox>()->Active()) {
                    std::stringstream s;
                    UpdateTextbox(i,s);
                    textBoxes[i]->Children()[0]->template GetComponent<TextBox>()->Text = s.str();
                }
            }
        }
        prev = (*this->field);
    }

    virtual void TextboxChanged(int index, std::string text) = 0;
    virtual void UpdateTextbox(int index, std::stringstream& stream) = 0;
    GameObject* textBoxes[Size];
    
    T* field;
    T prev;
    bool forceUpdate;
};

struct FieldInfoEditorFloat : public FieldInfoEditorTextboxes<float, 1> {
    void TextboxChanged(int index, std::string text) override {
        float value = (float)atof(text.c_str());
        (*field) = value;
    }
    void UpdateTextbox(int index, std::stringstream& stream) override {
        stream<<(*field);
    }
};

struct FieldInfoEditorInt : public FieldInfoEditorTextboxes<int, 1> {
    void TextboxChanged(int index, std::string text) override {
        int value = (int)atoi(text.c_str());
        (*field) = value;
    }
    void UpdateTextbox(int index, std::stringstream& stream) override {
        stream<<(*field);
    }
};

struct FieldInfoEditorVector3 : public FieldInfoEditorTextboxes<Vector3, 3> {
    void TextboxChanged(int index, std::string text) override {
        float value = (float)atof(text.c_str());
        (*field).Set(index, value);
    }
    void UpdateTextbox(int index, std::stringstream& stream) override {
        stream<<(*field)[index];
    }
};

template<> IFieldEditor* FieldEditorCreator<int>::Create() {
    return new FieldInfoEditorInt();
}

template<> IFieldEditor* FieldEditorCreator<float>::Create() {
    return new FieldInfoEditorFloat();
}

template<> IFieldEditor* FieldEditorCreator<Vector3>::Create() {
    return new FieldInfoEditorVector3();
}

TypeEditorTitle::Callback TypeEditorTitle::Title = [] (void* guiPtr, void* parentPtr, const std::string& title) -> void* {
    Gui* gui = static_cast<Gui*>(guiPtr);
    GameObject* parent = static_cast<GameObject*>(parentPtr);
    
    GameObject* labelGo = gui->CreateLabel(parent, 0, 0, 0, title, 15);
    labelGo->AddComponent<Layouter>()->Min = {20,20};
    labelGo->AddComponent<Layouter>()->Desired = {100,20};
    labelGo->AddComponent<Layouter>()->Max = {5000,20};
    Label* label = labelGo->GetComponent<Label>();
    labelGo->GetComponent<Colorable>()->Color = Colour::Black();
    label->HAlignment = Font::Center;
    label->VAlignment = Font::Middle;
    
    return 0;
};
