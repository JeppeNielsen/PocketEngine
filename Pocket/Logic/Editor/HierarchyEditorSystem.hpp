//
//  HierarchyEditorSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 02/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "HierarchyEditor.hpp"
#include "Transform.hpp"
#include "Sizeable.hpp"
#include "Gui.hpp"
#include "Droppable.hpp"

namespace Pocket {
    class HierarchyEditorSystem : public GameSystem<HierarchyEditor, Transform, Sizeable> {
        public:
            Gui* gui;
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
            void Update(float dt);
        private:
            void ObjectChanged(GameObject* object);
            int CountDepth(GameObject* object);
            void OnDropped(DroppedData d, GameObject* editorObject);
            bool IsParentLegal(GameObject* parent, GameObject* ancestor);
            GameWorld* world;
    };
}