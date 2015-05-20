//
//  GamePadController.h
//  Platformer
//
//  Created by Jeppe Nielsen on 27/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once

#include "GameWorld.hpp"
#include "GamePadManager.hpp"

using namespace Pocket;

Component(GamePadController)
    public:
        void Reset();
        int gamePadIndex;

};