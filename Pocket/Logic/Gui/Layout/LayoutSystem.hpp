//
//  LayoutSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/19/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Layoutable.hpp"
#include "Sizeable.hpp"
#include "Transform.hpp"
#include <set>

namespace Pocket {
    class LayoutSystem : public GameSystem<Transform, Sizeable, Layoutable> {
    public:
        void Update(float dt);
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
            
            void ParentChanged();
            void ParentSizeChanged();
            void SizeChanged();
            
            void IterateChildren(const ObjectCollection& children, std::function<void(Transform* transform, Sizeable* sizeable)> function);
        };
        
        typedef std::set<LayoutObject*> DirtyObjects;
        DirtyObjects dirtyObjects;
        DirtyObjects dirtyChildLayoutables;
    };
}