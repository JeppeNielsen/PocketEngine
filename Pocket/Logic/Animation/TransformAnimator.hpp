//
//  TransformAnimator.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "TransformAnimation.hpp"
#include "Property.hpp"

namespace Pocket {
    struct TransformAnimator {
    public:
        TransformAnimator();
    
        void Play(TransformAnimation* animation, float blendingDuration = 0);
    
        void Stop(float blendingDuration = 0);
    
        void SetFromLastFrame(TransformAnimation* animation);
    
        Property<TransformAnimation*> CurrentAnimation;
        TransformAnimation* previousAnimation;
    
        float progress[3];
        float previousProgress[3];
    
        float blendingProgress;
        float blendingDuration;
    };
}