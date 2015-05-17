//
//  SpineAnimator.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 05/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SpineAnimator.hpp"

using namespace Pocket;

SpineAnimator::SpineAnimator() : CurrentAnimation(this) {}

void SpineAnimator::Reset() {
    time = 0;
    blendingDuration = 0;
    blendingProgress = 0;
    CurrentAnimation = 0;
    previousAnimation = 0;
}

void SpineAnimator::Play(SpineAnimation *animation, float blendingDuration) {
    time = 0;
    previousAnimation = CurrentAnimation();
    CurrentAnimation = animation;
    blendingProgress = 0;
    this->blendingDuration = blendingDuration;
    previousTime = time;
}

void SpineAnimator::Stop(float blendingDuration) {
    Play(0,blendingDuration);
}
