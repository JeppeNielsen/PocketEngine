//
//  LayoutSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/19/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Layoutable.hpp"
#include "Sizeable.hpp"
#include "Transform.hpp"
#include <set>

namespace Pocket {
    class LayoutSystem : public GameSystem {
    
    public:
        void Update(float dt);
        
        void Initialize();
    
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);

    private:
        
        struct LayoutObject {
            LayoutObject(GameObject* object, LayoutSystem* layoutSystem);
            ~LayoutObject();
            LayoutSystem* layoutSystem;
            Sizeable* sizeable;
            GameObject* object;
            Layoutable* layoutable;
            Transform* transform;
            Vector2 deltaSize;
            Vector2 oldSize;
            Sizeable* parentSizeable;
            LayoutObject* parentLayoutObject;
            
            void Update();
            void UpdateChildren();
            
            void ParentChanged(Property<GameObject*, GameObject*>::EventData d);
            void ParentSizeChanged(Property<Sizeable*, Vector2>::EventData d);
            
            void IterateChildren(std::function<void(Transform* transform, Sizeable* sizeable)> function);
        };
        
        typedef std::set<LayoutObject*> DirtyObjects;
        DirtyObjects dirtyObjects;
        DirtyObjects dirtyChildLayoutables;
    };
}