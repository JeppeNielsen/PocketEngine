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
    struct DroppableSystem : GameSystem<Droppable, Touchable> {
        using Systems = meta::list<TouchSystem>;
        void Initialize(IGameWorld* world);
        void ObjectAdded(GameObject *object);
        void ObjectRemoved(GameObject *object);
        void TouchUp(Pocket::TouchData d, GameObject* object);
        TouchSystem* touchSystem;
    };
}