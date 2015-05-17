//
//  SpineAnimator.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 05/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "SpineAnimation.hpp"


namespace Pocket {
    Component(SpineAnimator)
    public:
        SpineAnimator();

        void Reset();

        void Play(SpineAnimation* animation, float blendingDuration = 0);

        void Stop(float blendingDuration = 0);

        Property<SpineAnimator*, SpineAnimation*> CurrentAnimation;
        SpineAnimation* previousAnimation;

        float time;
        float previousTime;

        float blendingProgress;
        float blendingDuration;
    };
}