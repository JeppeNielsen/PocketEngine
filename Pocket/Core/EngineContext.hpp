//
//  GameManager.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 9/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "Vector2.hpp"
#include "Box.hpp"

namespace Pocket {
    class EngineContext {
    public:
        Property<Vector2> ScreenSize;
        Box Viewport() const;
    };
}