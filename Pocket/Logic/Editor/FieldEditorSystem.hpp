//
//  FieldEditorSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "FieldEditor.hpp"
#include "Sizeable.hpp"
#include "Transform.hpp"
#include "Gui.hpp"

namespace Pocket {
class FieldEditorSystem : public GameSystem<FieldEditor, Sizeable, Transform> {
    public:
        Gui* gui;
    
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
        void FieldChanged(GameObject* object);
    };
}