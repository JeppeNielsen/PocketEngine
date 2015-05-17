//
//  TextBoxSystem.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TextBoxSystem.hpp"
#include "Label.hpp"

using namespace Pocket;

void TextBoxSystem::Initialize() {
    activeTextBox = 0;
    AddComponent<TextBox>();
    AddComponent<Touchable>();
}

void TextBoxSystem::ObjectAdded(Pocket::GameObject *object) {
    Touchable* touchable = object->GetComponent<Touchable>();
    touchable->Down += event_handler(this, &TextBoxSystem::TouchDown, object);
    touchable->Up += event_handler(this, &TextBoxSystem::TouchUp, object);
    touchable->Click += event_handler(this, &TextBoxSystem::TouchClick, object);
    TextBox* textBox = object->GetComponent<TextBox>();
    textBox->Active.Changed += event_handler(this, &TextBoxSystem::ActiveTextBoxChanged);
    if (textBox->Active) {
        ActiveTextBoxChanged(textBox);
    }
}

void TextBoxSystem::ObjectRemoved(Pocket::GameObject *object) {
    Touchable* touchable = object->GetComponent<Touchable>();
    touchable->Down -= event_handler(this, &TextBoxSystem::TouchDown, object);
    touchable->Up -= event_handler(this, &TextBoxSystem::TouchUp, object);
    touchable->Click -= event_handler(this, &TextBoxSystem::TouchClick, object);
    TextBox* textBox = object->GetComponent<TextBox>();
    textBox->Active = false;
    textBox->Active.Changed -= event_handler(this, &TextBoxSystem::ActiveTextBoxChanged);
}

void TextBoxSystem::ActiveTextBoxChanged(TextBox* textBox) {
    if (textBox->Active) {
        activeTextBox = textBox;
        for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
            TextBox* textBoxToDisable = (*it)->GetComponent<TextBox>();
            if (textBox!=textBoxToDisable) {
                textBoxToDisable->Active = false;
            }
        }
        
        Input->KeyboardText = textBox->Text;
        Input->KeyboardActive = true;
        
        Input->KeyboardActive.Changed += event_handler(this, &TextBoxSystem::KeyboardActiveChanged);
        Input->KeyboardText.Changed += event_handler(this, &TextBoxSystem::KeyboardTextChanged);
        Input->TouchUp += event_handler(this, &TextBoxSystem::TouchInputUp);
        
    } else {
        if (textBox == activeTextBox) {
            activeTextBox = 0;
            Input->KeyboardActive.Changed -= event_handler(this, &TextBoxSystem::KeyboardActiveChanged);
            Input->KeyboardText.Changed -= event_handler(this, &TextBoxSystem::KeyboardTextChanged);
            Input->TouchUp -= event_handler(this, &TextBoxSystem::TouchInputUp);
            Input->KeyboardActive = false;
        }
    }
}

void TextBoxSystem::TouchInputUp(Pocket::TouchEvent e) {
    touchWasUp = true;
}

void TextBoxSystem::TouchDown(Pocket::TouchData d, Pocket::GameObject *object) {
    pushedTextBoxes[object] = { d, d.Position, 0 };
}

void TextBoxSystem::TouchUp(Pocket::TouchData d, Pocket::GameObject *object) {
    PushedTextBoxes::iterator it = pushedTextBoxes.find(object);
    if (it!=pushedTextBoxes.end()) {
        pushedTextBoxes.erase(it);
    }
    anyTextboxUp = true;
}

void TextBoxSystem::TouchClick(Pocket::TouchData d, Pocket::GameObject *object) {
    PushedTextBoxes::iterator it = pushedTextBoxes.find(object);
    if (it!=pushedTextBoxes.end()) {
        if (it->second.distanceMoved<30) {
            object->GetComponent<TextBox>()->Active = true;
        }
    }
}

void TextBoxSystem::Update(float dt) {
    
    if (touchWasUp && !anyTextboxUp) {
        if (activeTextBox) {
            activeTextBox->Active = false;
        }
    }
    touchWasUp = false;
    anyTextboxUp = false;
    
    if (pushedTextBoxes.empty()) return;
    
    for (PushedTextBoxes::iterator it = pushedTextBoxes.begin(); it!=pushedTextBoxes.end(); ++it) {
        Vector2 touchPosition = it->second.touch.Input->GetTouchPosition(it->second.touch.Index);
        Vector2 delta = touchPosition-it->second.lastTouchPosition;
        it->second.distanceMoved += delta.Length();
        it->second.lastTouchPosition = touchPosition;
    }
}

void TextBoxSystem::KeyboardActiveChanged(Pocket::InputManager *input) {
    if (!input->KeyboardActive && activeTextBox) {
        activeTextBox->Active = false;
    }
}

void TextBoxSystem::KeyboardTextChanged(Pocket::InputManager *input) {
    if (activeTextBox) {
        activeTextBox->Text = input->KeyboardText;
    }
}



