//
//  TextBoxSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TextBoxSystem.hpp"

using namespace Pocket;

void TextBoxSystem::Initialize() {
    ActiveTextBox = 0;
}

void TextBoxSystem::ObjectAdded(GameObject *object) {
    Touchable* touchable = object->GetComponent<Touchable>();
    touchable->Down.Bind(this, &TextBoxSystem::TouchDown, object);
    touchable->Up.Bind(this, &TextBoxSystem::TouchUp, object);
    touchable->Click.Bind(this, &TextBoxSystem::TouchClick, object);
    TextBox* textBox = object->GetComponent<TextBox>();
    textBox->Active.Changed.Bind(this, &TextBoxSystem::ActiveTextBoxChanged, object);
    if (textBox->Active) {
        ActiveTextBoxChanged(object);
    }
}

void TextBoxSystem::ObjectRemoved(GameObject *object) {
    Touchable* touchable = object->GetComponent<Touchable>();
    touchable->Down.Unbind(this, &TextBoxSystem::TouchDown, object);
    touchable->Up.Unbind(this, &TextBoxSystem::TouchUp, object);
    touchable->Click.Unbind(this, &TextBoxSystem::TouchClick, object);
    TextBox* textBox = object->GetComponent<TextBox>();
    textBox->Active = false;
    textBox->Active.Changed.Unbind(this, &TextBoxSystem::ActiveTextBoxChanged, object);
}

void TextBoxSystem::ActiveTextBoxChanged(GameObject* object) {
    TextBox* textBox = object->GetComponent<TextBox>();
    
    if (textBox->Active) {
        ActiveTextBox = textBox;
        for(auto o : this->Objects()) {
            TextBox* textBoxToDisable = o->GetComponent<TextBox>();
            if (textBox!=textBoxToDisable) {
                textBoxToDisable->Active = false;
            }
        }
        
        root->Input().KeyboardText = textBox->Text;
        root->Input().KeyboardActive = true;
        
        root->Input().KeyboardActive.Changed.Bind(this, &TextBoxSystem::KeyboardActiveChanged);
        root->Input().KeyboardText.Changed.Bind(this, &TextBoxSystem::KeyboardTextChanged);
        root->Input().TouchDown.Bind(this, &TextBoxSystem::TouchInputUp);
        
    } else {
        if (textBox == ActiveTextBox()) {
            ActiveTextBox = 0;
            root->Input().KeyboardActive.Changed.Unbind(this, &TextBoxSystem::KeyboardActiveChanged);
            root->Input().KeyboardText.Changed.Unbind(this, &TextBoxSystem::KeyboardTextChanged);
            root->Input().TouchDown.Unbind(this, &TextBoxSystem::TouchInputUp);
            root->Input().KeyboardActive = false;
        }
    }
}

void TextBoxSystem::TouchInputUp(Pocket::TouchEvent e) {
    touchWasUp = true;
}

void TextBoxSystem::TouchDown(Pocket::TouchData d, GameObject *object) {
    pushedTextBoxes[object] = { d, d.Position, 0 };
}

void TextBoxSystem::TouchUp(Pocket::TouchData d, GameObject *object) {
    auto it = pushedTextBoxes.find(object);
    if (it!=pushedTextBoxes.end()) {
        pushedTextBoxes.erase(it);
    }
    anyTextboxUp = true;
}

void TextBoxSystem::TouchClick(Pocket::TouchData d, GameObject *object) {
    auto it = pushedTextBoxes.find(object);
    if (it!=pushedTextBoxes.end()) {
        if (it->second.distanceMoved<30) {
            object->GetComponent<TextBox>()->Active = true;
        }
    }
}

void TextBoxSystem::Update(float dt) {
    
    if (touchWasUp && !anyTextboxUp) {
        if (ActiveTextBox()) {
            ActiveTextBox()->Active = false;
        }
    }
    touchWasUp = false;
    anyTextboxUp = false;
    
    if (pushedTextBoxes.empty()) return;
    
    for (auto it = pushedTextBoxes.begin(); it!=pushedTextBoxes.end(); ++it) {
        Vector2 touchPosition = it->second.touch.Input->GetTouchPosition(it->second.touch.Index);
        Vector2 delta = touchPosition-it->second.lastTouchPosition;
        it->second.distanceMoved += delta.Length();
        it->second.lastTouchPosition = touchPosition;
    }
}

void TextBoxSystem::KeyboardActiveChanged() {
    if (!root->Input().KeyboardActive && ActiveTextBox() && ActiveTextBox()->DeselectOnEnter) {
        ActiveTextBox()->Active = false;
    }
}

void TextBoxSystem::KeyboardTextChanged() {
    if (ActiveTextBox()) {
        ActiveTextBox()->Text = root->Input().KeyboardText;
    }
}
        
