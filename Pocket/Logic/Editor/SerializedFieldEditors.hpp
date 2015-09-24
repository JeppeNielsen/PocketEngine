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
    
    void CreateDefaultSerializedEditors();
}
