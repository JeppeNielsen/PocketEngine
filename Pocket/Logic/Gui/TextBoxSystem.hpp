//
//  TextBoxSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "TextBox.hpp"
#include "InputManager.hpp"
#include "Touchable.hpp"
#include <map>

namespace Pocket {
    class TextBoxSystem : public GameSystem<TextBox, Touchable> {
    public:
        using GameObject = GameObject<T>;
 
        void Initialize() {
            ActiveTextBox = 0;
        }

        void ObjectAdded(GameObject *object) {
            Touchable* touchable = object->GetComponent<Touchable>();
            touchable->Down.Bind(this, &TextBoxSystem::TouchDown, object);
            touchable->Up.Bind(this, &TextBoxSystem::TouchUp, object);
            touchable->Click.Bind(this, &TextBoxSystem::TouchClick, object);
            TextBox* textBox = object->GetComponent<TextBox>();
            textBox->Active.Changed.Bind(this, &TextBoxSystem::ActiveTextBoxChanged);
            if (textBox->Active) {
                ActiveTextBoxChanged(textBox);
            }
        }

        void ObjectRemoved(GameObject *object) {
            Touchable* touchable = object->GetComponent<Touchable>();
            touchable->Down.Unbind(this, &TextBoxSystem::TouchDown, object);
            touchable->Up.Unbind(this, &TextBoxSystem::TouchUp, object);
            touchable->Click.Unbind(this, &TextBoxSystem::TouchClick, object);
            TextBox* textBox = object->GetComponent<TextBox>();
            textBox->Active = false;
            textBox->Active.Changed.Unbind(this, &TextBoxSystem::ActiveTextBoxChanged);
        }

        void ActiveTextBoxChanged(TextBox* textBox) {
            if (textBox->Active) {
                ActiveTextBox = textBox;
                for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
                    TextBox* textBoxToDisable = (*it)->GetComponent<TextBox>();
                    if (textBox!=textBoxToDisable) {
                        textBoxToDisable->Active = false;
                    }
                }
                
                Input->KeyboardText = textBox->Text;
                Input->KeyboardActive = true;
                
                Input->KeyboardActive.Changed.Bind(this, &TextBoxSystem::KeyboardActiveChanged);
                Input->KeyboardText.Changed.Bind(this, &TextBoxSystem::KeyboardTextChanged);
                Input->TouchDown.Bind(this, &TextBoxSystem::TouchInputUp);
                
            } else {
                if (textBox == ActiveTextBox()) {
                    ActiveTextBox = 0;
                    Input->KeyboardActive.Changed.Unbind(this, &TextBoxSystem::KeyboardActiveChanged);
                    Input->KeyboardText.Changed.Unbind(this, &TextBoxSystem::KeyboardTextChanged);
                    Input->TouchDown.Unbind(this, &TextBoxSystem::TouchInputUp);
                    Input->KeyboardActive = false;
                }
            }
        }

        void TouchInputUp(Pocket::TouchEvent e) {
            touchWasUp = true;
        }

        void TouchDown(Pocket::TouchData d, GameObject *object) {
            pushedTextBoxes[object] = { d, d.Position, 0 };
        }

        void TouchUp(Pocket::TouchData d, GameObject *object) {
            PushedTextBoxes::iterator it = pushedTextBoxes.find(object);
            if (it!=pushedTextBoxes.end()) {
                pushedTextBoxes.erase(it);
            }
            anyTextboxUp = true;
        }

        void TouchClick(Pocket::TouchData d, GameObject *object) {
            PushedTextBoxes::iterator it = pushedTextBoxes.find(object);
            if (it!=pushedTextBoxes.end()) {
                if (it->second.distanceMoved<30) {
                    object->GetComponent<TextBox>()->Active = true;
                }
            }
        }

        void Update(float dt) {
            
            if (touchWasUp && !anyTextboxUp) {
                if (ActiveTextBox()) {
                    ActiveTextBox()->Active = false;
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

        void KeyboardActiveChanged(Pocket::InputManager *input) {
            if (!input->KeyboardActive && ActiveTextBox()) {
                ActiveTextBox()->Active = false;
            }
        }

        void KeyboardTextChanged(Pocket::InputManager *input) {
            if (ActiveTextBox()) {
                ActiveTextBox()->Text = input->KeyboardText;
            }
        }
                
                
        
        
        struct pushedTextBox {
            TouchData touch;
            Vector2 lastTouchPosition;
            float distanceMoved;
        };
        
        typedef std::map<GameObject*, pushedTextBox> PushedTextBoxes;
        PushedTextBoxes pushedTextBoxes;
        
        bool touchWasUp;
        bool anyTextboxUp;
        
        InputManager* Input;
        Property<TextBox*> ActiveTextBox;
        
    };
}