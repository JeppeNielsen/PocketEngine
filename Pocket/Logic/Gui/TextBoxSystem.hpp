//
//  TextBoxSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "TextBox.hpp"
#include "InputManager.hpp"
#include "Touchable.hpp"
#include <map>

namespace Pocket {
    class TextBoxSystem : public GameSystem {
    public:

        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
        
        InputManager* Input;
        
    private:
        TextBox* activeTextBox;
        
        void ActiveTextBoxChanged(TextBox* textBox);
        
        void KeyboardActiveChanged(InputManager* input);
        void KeyboardTextChanged(InputManager* input);
        
        void TouchInputUp(TouchEvent e);
        
        void TouchDown(TouchData d, GameObject* object);
        void TouchUp(TouchData d, GameObject* object);
        void TouchClick(TouchData d, GameObject* object);
        
        struct pushedTextBox {
            TouchData touch;
            Vector2 lastTouchPosition;
            float distanceMoved;
        };
        
        typedef std::map<GameObject*, pushedTextBox> PushedTextBoxes;
        PushedTextBoxes pushedTextBoxes;
        
        bool touchWasUp;
        bool anyTextboxUp;
        
    };
}