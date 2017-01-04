//
//  DraggableVelocitySystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Draggable.hpp"
#include "DraggableMotion.hpp"
#include "Velocity.hpp"
#include "Transform.hpp"
#include <map>
#include <vector>

namespace Pocket {
    class DraggableMotionSystem : public GameSystem<Transform, Velocity, Draggable> {
    public:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
        
    private:
        
        void DraggingChanged(GameObject* object);
    
        struct DraggingData {
            GameObject* object;
            Vector3 lastPosition;
            std::vector<Vector3> velocities;
        };
        
        typedef std::map<Draggable*, DraggingData> DraggingObjects;
        DraggingObjects draggingObjects;
        
    };
}