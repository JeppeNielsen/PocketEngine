//
//  FirstPersonMover.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Quaternion.hpp"

namespace Pocket {
    class FirstPersonMover {
    public:
        FirstPersonMover();
        float MovementSpeed;
        float RotationSpeed;
        float MaxPitch;
        Quaternion initialRotation;
        int MovementTouchIndex;
        int RotationTouchIndex;
        
        void SetTouchIndices(int movementTouchIndex, int rotationTouchIndex);
    };
}
