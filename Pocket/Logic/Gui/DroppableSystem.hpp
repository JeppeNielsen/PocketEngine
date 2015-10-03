//
//  DroppableSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Droppable.hpp"
#include "TouchSystem.hpp"

namespace Pocket {
  SYSTEM(DroppableSystem, Droppable, Touchable)
    public:
        void AddedToWorld(GameWorld& world);
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
    private:
        void TouchUp(TouchData d, GameObject* object);
        TouchSystem* touchSystem;
  };
}