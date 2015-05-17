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

    
    struct MovementMode {
        enum Type {
            Normal,
            XAxis,
            YAxis,
            ZAxis,
            XYPlane,
            YZPlane,
            XZPlane,
        };
    };
    
    MovementMode::Type Movement;
    Property<Draggable*, bool> IsDragging;
    
    };
}