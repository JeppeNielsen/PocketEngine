//
//  SpineAnimation.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 05/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SpineAnimation.hpp"

using namespace Pocket;

void SpineAnimation::CalcStartEndTime() {
    startTime = 1000000.0f;
    endTime = 0.0f;
    
    for (Slots::iterator it = slots.begin(); it!=slots.end(); ++it) {
        CalcMinMaxTime(it->second, startTime, endTime);
    }
    for (Bones::iterator it = bones.begin(); it!=bones.end(); ++it) {
        CalcMinMaxTime(it->second.position, startTime, endTime);
        CalcMinMaxTime(it->second.rotation, startTime, endTime);
        CalcMinMaxTime(it->second.scale, startTime, endTime);
    }
}