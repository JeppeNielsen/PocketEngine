//
//  TextBoxLabelSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Sizeable.hpp"
#include "TextBox.hpp"
#include "Label.hpp"
#include "Mesh.hpp"
#include "Renderable.hpp"
#include "Sizeable.hpp"

namespace Pocket {
    class TextBoxLabelSystem : public GameSystem<TextBox, Label, Mesh, Sizeable> {
    public:
        
        const float cursorWidth = 2.0f;
    
        void Initialize();
        void ObjectAdded(GameObject *object);
        void ObjectRemoved(GameObject *object);
        void TextBoxChanged(GameObject* object);
        void TextBoxActiveChanged(GameObject *object);
        void MoveCursor(GameObject *object);
        void Update(float dt);

    private:
        GameObject* cursor;
        TextBox* activeTextbox;
        TextBox* activeTextboxAdded;
        GameObject* activeTextureObject;
        GameObject* activeTextboxAddedGO;
        float timer;
    };
}
