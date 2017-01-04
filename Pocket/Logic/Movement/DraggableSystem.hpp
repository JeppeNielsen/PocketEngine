//
//  DraggerSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Draggable.hpp"
#include "Touchable.hpp"
#include "Transform.hpp"
#include "Plane.hpp"
#include <map>

namespace Pocket {
    class DraggableSystem : public GameSystem<Transform, Touchable, Draggable> {
    public:
        void ObjectAdded(GameObject *object);
        void ObjectRemoved(GameObject *object);
        void Update(float dt);
        bool IsTouchIndexUsed(int touchIndex);
    private:
        struct DraggingObject {
            TouchData touch;
            Vector3 offset;
            Plane dragPlane;
            Vector3 touchPosition;
            
            Touchable* touchable;
            Transform* transform;
            Draggable* draggable;
        };
        
        typedef std::map<Touchable*, DraggingObject> DraggingObjects;
        DraggingObjects draggingObjects;
        
        void Down(TouchData d, GameObject* object);
        void Up(TouchData d, GameObject* object);
    };
}