//
//  Dragger.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"

namespace Pocket {
    struct Draggable {
        
        Draggable() : Movement(MovementMode::Normal) { IsDragging = false; }
        
        enum class MovementMode {
            Normal,
            XAxis,
            YAxis,
            ZAxis,
            XYPlane,
            YZPlane,
            XZPlane,
        };
        
        MovementMode Movement;
        Property<bool> IsDragging;
    };
}