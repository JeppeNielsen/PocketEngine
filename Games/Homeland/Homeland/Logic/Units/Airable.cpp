//
//  Airable.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 28/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Airable.h"
#include "MathHelper.hpp"

using namespace Pocket;

void Airable::Reset() {
    alignmentSpeed = 5.0f;
    positions.clear();
    targetAboveGround = 2;
    currentHeightY=0;
    heightAligmentSpeed=1.0f;
    sineWobble = MathHelper::Random() * MathHelper::Pi * 2;
}