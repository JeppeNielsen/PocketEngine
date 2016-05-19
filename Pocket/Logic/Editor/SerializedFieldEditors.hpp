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
#include "TypeInfo.hpp"

namespace Pocket {
    struct SerializedFieldEditorFloat : public FieldInfoEditor<float, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void TextChanged(GameObject* object);
        void Update(float dt);
        GameObject* textBox;
        float prev;
    };
    
    struct SerializedFieldEditorInt : public FieldInfoEditor<int, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void TextChanged(GameObject* object);
        void Update(float dt);
        GameObject* textBox;
        int prev;
    };
    
    struct SerializedFieldEditorVector2 : public FieldInfoEditor<Vector2, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void TextChanged(GameObject* object);
        void Update(float dt);
        GameObject* textBox[2];
        Vector2 prev;
    };

    struct SerializedFieldEditorVector3 : public FieldInfoEditor<Vector3, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void TextChanged(GameObject* object);
        void Update(float dt);
        GameObject* textBox[3];
        Vector3 prev;
    };
    
    struct SerializedFieldEditorString : public FieldInfoEditor<std::string, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void TextChanged(GameObject* object);
        void Update(float dt);
        GameObject* textBox;
        std::string prev;
    };
    
    struct SerializedFieldEditorBool : public FieldInfoEditor<bool, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void Clicked(TouchData touch);
        void Update(float dt);
        GameObject* box;
        GameObject* tick;
        bool prev;
    };
    
    struct SerializedFieldEditorQuaternion : public FieldInfoEditor<Quaternion, Gui, GameObject> {
        void Initialize(Gui* context, GameObject* parent);
        void Destroy();
        void TextChanged(GameObject* object);
        void Update(float dt);
        GameObject* textBox[3];
        Vector3 prev;
    };
    
    void CreateDefaultSerializedEditors();
}
