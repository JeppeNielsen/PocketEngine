//
//  DroppableSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Droppable.hpp"
#include "DroppableSystem.hpp"
#include "TouchSystem.hpp"

namespace Pocket {
    class DroppableSystem : public GameSystem<Droppable, Touchable> {
    public:
        
        void Initialize();
        void ObjectAdded(GameObject *object);
        void ObjectRemoved(GameObject *object);
        void Update(float dt);
        static void CreateSubSystems(GameStorage& storage);
    private:
    
        void TouchDown(TouchData d, GameObject* object);
        void DropStarted(TouchData d, GameObject* object);
        void TouchUp(TouchData d, GameObject* object);
        TouchSystem* touchSystem;
        
        struct DownObject {
            float maxMovement;
            TouchData touchData;
            Vector2 prevPosition;
            GameObject* createdObject;
        };
        
        std::map<GameObject*, DownObject> downObjects;
    };
}
