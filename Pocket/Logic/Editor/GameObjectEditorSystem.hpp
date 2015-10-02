//
//  GameObjectEditorSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "GameObjectEditor.hpp"
#include "Transform.hpp"
#include "Sizeable.hpp"
#include "Gui.hpp"

namespace Pocket {
    SYSTEM(GameObjectEditorSystem, GameObjectEditor, Transform, Sizeable)
        public:
            Gui* gui;
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
        private:
            void ObjectChanged(GameObjectEditor* editor, GameObject* object);
    
    };
}