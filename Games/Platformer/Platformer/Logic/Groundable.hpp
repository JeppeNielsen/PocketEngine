//
//  Groundable.h
//  Platformer
//
//  Created by Jeppe Nielsen on 29/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"
#include "Property.hpp"

using namespace Pocket;

Component(Groundable)
    public:
        Groundable();
        void Reset();
        Property<Groundable*, bool> IsGrounded;
};