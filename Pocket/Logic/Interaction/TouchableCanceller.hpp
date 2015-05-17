//
//  TouchableCanceller.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/11/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameComponent.hpp"

namespace Pocket {
    Component(TouchableCanceller)
    public:
        void Reset();
        float MovementToCancel;
    };
}