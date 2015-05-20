//
//  CharacterControllerSystem.h
//  Platformer
//
//  Created by Jeppe Nielsen on 27/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "CharacterController.hpp"
#include "RigidBody2d.hpp"

using namespace Nano;

SYSTEM(CharacterControllerSystem, CharacterController, RigidBody2d)
public:
    void Update(float dt);
};