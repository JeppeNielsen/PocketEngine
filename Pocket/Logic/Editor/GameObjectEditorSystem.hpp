//
//  GameObjectEditorSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "GameObjectEditor.hpp"
#include "Transform.hpp"
#include "Sizeable.hpp"
#include "Gui.hpp"
#include <set>
#ifdef SCRIPTING_ENABLED
#include "ScriptWorld.hpp"
#endif

namespace Pocket {
    class GameObjectEditorSystem : public GameSystem<GameObjectEditor, Transform, Sizeable> {
        public:
            GameObjectEditorSystem();
            void Initialize();
        
            Gui* gui;
#ifdef SCRIPTING_ENABLED
            ScriptWorld* scriptWorld;
#endif
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
        
            template<class T>
            void IgnoreComponent() {
                ignoredComponents.push_back(GameIdHelper::GetComponentID<T>());
            }
            void Update(float dt);
        
            std::function<bool(int componentID)> Predicate;
        
        private:
            void ObjectChanged(GameObject* object);
            void CreateEditors(GameObject* object);
            typedef std::vector<int> IgnoredComponents;
            IgnoredComponents ignoredComponents;
        
            using DirtyObjects = std::set<GameObject*>;
            DirtyObjects dirtyObjects;
    };
}