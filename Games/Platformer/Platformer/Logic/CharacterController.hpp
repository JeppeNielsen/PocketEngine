//
//  CharacterController.h
//  Platformer
//
//  Created by Jeppe Nielsen on 27/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Vector2.hpp"

using namespace Pocket;

Component(CharacterController)
    public:
        void Reset();
        Vector2 movement;
};
