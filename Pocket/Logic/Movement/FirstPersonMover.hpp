//
//  FirstPersonMover.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameComponent.hpp"
#include "Quaternion.hpp"

namespace Pocket {
    Component(FirstPersonMover)
public:
    float MovementSpeed;
    float RotationSpeed;
    float MaxPitch;
    
    Quaternion initialRotation;
    
    void Reset();
    void Clone(const FirstPersonMover& other);
    
    };
}
