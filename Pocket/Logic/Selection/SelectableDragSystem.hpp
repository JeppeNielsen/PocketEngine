//
//  SelectableDragSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/26/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameSystem.hpp"
#include "Draggable.hpp"
#include "Selectable.hpp"
#include "Transform.hpp"
#include <map>
#include <vector>

namespace Pocket {
    class SelectableDragSystem : public GameSystem<Draggable, Selectable, Transform>  {
    public:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
    private:
        void IsDraggingChanged(GameObject* object);
        
        struct DragData {
            std::vector<GameObject*> objects;
            std::vector<Vector3> offsets;
        };
        
        typedef std::map<GameObject*, DragData> DraggingObjects;
        DraggingObjects draggingObjects;
    };
}