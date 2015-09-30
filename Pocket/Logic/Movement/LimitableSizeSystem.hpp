//
//  LimitableSizeSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 30/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Limitable.hpp"
#include "Sizeable.hpp"

namespace Pocket {
    SYSTEM(LimitableSizeSystem, Limitable, Sizeable)
    public:
        void Update(float dt);
    };
}