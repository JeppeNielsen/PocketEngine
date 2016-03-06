//
//  TextBoxLabelSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Sizeable.hpp"
#include "TextBox.hpp"
#include "Label.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Sizeable.hpp"

namespace Pocket {
    class TextBoxLabelSystem : public GameSystem<TextBox, Label, Mesh, Sizeable> {
    public:
        
        const float cursorWidth = 1.05f;
    
        void Initialize(GameWorld* world);
        void ObjectAdded(GameObject *object);
        void ObjectRemoved(GameObject *object);
        void TextBoxChanged(GameObject* object);
        void TextBoxActiveChanged(bool& active, GameObject *object);
        void MoveCursor(Pocket::TextBox *textBox, GameObject *object);
        void Update(float dt);

    private:
        GameObject* cursor;
        TextBox* activeTextbox;
        TextBox* activeTextboxAdded;
        GameObject* activeTextboxAddedGO;
        float timer;
        GameWorld* world;
    };
}