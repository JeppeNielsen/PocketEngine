//
//  SerializedFieldEditors.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Gui.hpp"
#include "TextBox.hpp"

namespace Pocket {
    struct SerializedFieldEditorFloat : public SerializedFieldEditor<float, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void TextChanged(TextBox* textBox);
        void Update(float dt);
        GameObject* textBox;
        float prev;
    };
    
    struct SerializedFieldEditorVector2 : public SerializedFieldEditor<Vector2, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void TextChanged(TextBox* textBox, GameObject* object);
        void Update(float dt);
        GameObject* textBox[2];
        Vector2 prev;
    };

    struct SerializedFieldEditorVector3 : public SerializedFieldEditor<Vector3, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void TextChanged(TextBox* textBox, GameObject* object);
        void Update(float dt);
        GameObject* textBox[3];
        Vector3 prev;
    };
    
    struct SerializedFieldEditorString : public SerializedFieldEditor<std::string, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void TextChanged(TextBox* textBox);
        void Update(float dt);
        GameObject* textBox;
        std::string prev;
    };
    
    struct SerializedFieldEditorBool : public SerializedFieldEditor<bool, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void Clicked(TouchData touch);
        void Update(float dt);
        GameObject* box;
        GameObject* tick;
        bool prev;
    };
    
    void CreateDefaultSerializedEditors();
}
