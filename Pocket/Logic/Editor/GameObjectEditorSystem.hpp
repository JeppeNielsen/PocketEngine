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
#include "ScriptWorld.hpp"

namespace Pocket {
    class GameObjectEditorSystem : public GameSystem<GameObjectEditor, Transform, Sizeable> {
        public:
            GameObjectEditorSystem();
            void Initialize(GameWorld* world);
        
            Gui* gui;
            ScriptWorld* scriptWorld;
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
        
            template<class T>
            void IgnoreComponent() {
                ignoredComponents.push_back(IDHelper::GetComponentID<T>());
            }
        private:
            GameWorld* world;
            void ObjectChanged(GameObject* object);
            typedef std::vector<int> IgnoredComponents;
            IgnoredComponents ignoredComponents;
    };
}