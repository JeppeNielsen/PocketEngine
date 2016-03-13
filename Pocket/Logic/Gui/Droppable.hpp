//
//  Droppable.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "Touchable.hpp"

namespace Pocket {
    struct DroppedData {
        GameObject* object;
        TouchData touchData;
        std::vector<TouchData> droppedTouches;
    };

    struct Droppable {
        Event<DroppedData> Dropped;
    };
}