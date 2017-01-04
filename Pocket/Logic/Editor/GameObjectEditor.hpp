//
//  GameObjectEditor.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "GameObject.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    class GameObjectEditor {
    public:
        GameObjectEditor();
        Property<GameObject*> Object;

        struct ComponentCreatedData {
            TypeInfo typeInfo;
            GameObject* editorPivot;
        };
        
        std::vector<GameObject::ComponentEditor> editors;
        
        Event<ComponentCreatedData> ComponentEditorCreated;
    };
}
