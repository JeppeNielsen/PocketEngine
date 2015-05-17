//
//  TextBoxLabelSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "TextBox.hpp"
#include "Label.hpp"

namespace Pocket {
    class TextBoxLabelSystem : public GameSystem {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
        
    private:
        
        
        
        void TextBoxTextChanged(TextBox* textBox, GameObject* object);
        void TextBoxActiveChanged(TextBox* textBox, GameObject* object);
        
        GameObject* cursor;
        
        TextBox* activeTextbox;
        
        TextBox* activeTextboxAdded;
        GameObject* activeTextboxAddedGO;
        
        void MoveCursor(TextBox* textBox, GameObject* object);
        
        float timer;
        
    };
}