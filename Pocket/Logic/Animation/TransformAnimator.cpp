//
//  TransformAnimator.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TransformAnimator.hpp"

using namespace Pocket;

TransformAnimator::TransformAnimator() {
    for (int i=0; i<3; ++i) {
        progress[i]=previousProgress[i]=0;
    }
    blendingDuration = 0;
    blendingProgress = 0;
    CurrentAnimation = 0;
    previousAnimation = 0;
}

void TransformAnimator::Play(Pocket::TransformAnimation *animation, float blendingDuration) {
    previousAnimation = CurrentAnimation();
    CurrentAnimation = animation;
    blendingProgress = 0;
    this->blendingDuration = blendingDuration;
    for (int i=0; i<3; ++i) {
        previousProgress[i]=progress[i];
        progress[i]=0;
    }
}

void TransformAnimator::Stop(float blendingDuration) {
    Play(0,blendingDuration);
}

void TransformAnimator::SetFromLastFrame(Pocket::TransformAnimation *animation) {
    previousAnimation = 0;
    CurrentAnimation = animation;
    blendingProgress = 0;
    blendingDuration = 0;
    for (int i=0; i<3; ++i) {
        previousProgress[i]=progress[i];
    }
    if (animation) {
        animation->GetPercentageProgresses(&progress[0], 1.0f);
    } else {
        progress[0] = 0;
        progress[1] = 0;
        progress[2] = 0;
    }
}

