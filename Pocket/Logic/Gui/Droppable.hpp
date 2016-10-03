//
//  Droppable.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "Touchable.hpp"
#include <functional>

namespace Pocket {
    struct DroppedData {
        GameObject* object;
        TouchData touchData;
        std::vector<TouchData> droppedTouches;
    };

    struct Droppable {
        Droppable() { activateThreshhold = 10.0f; }
        Event<DroppedData> Dropped;
        std::function<GameObject*(GameObject*, TouchData)> OnCreate;
        float activateThreshhold;
    };
}