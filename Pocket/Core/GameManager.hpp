//
//  GameManager.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 9/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Vector2.hpp"
#include "Box.hpp"

namespace Pocket {
    class Engine;
    class GameManager {
    public:
        
        Box Viewport() const;
        Vector2 ScreenSize() const;
        
    private:
        Vector2 screenSize;
        
        friend class Engine;
    };
}