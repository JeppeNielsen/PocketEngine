//
//  Dragger.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Property.hpp"

namespace Pocket {
    Component(Draggable)
public:
    
    Draggable();
    
    void Reset();
    void Clone(const Draggable& other);

    
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
    Property<Draggable*, bool> IsDragging;
    
    };
}