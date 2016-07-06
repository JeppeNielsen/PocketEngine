//
//  Airable.h
//  Homeland
//
//  Created by Jeppe Nielsen on 28/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Vector2.hpp"

using namespace Pocket;

struct Airable {
    void Reset();
    float alignmentSpeed;
    std::vector<Vector2> positions;
    float targetAboveGround;
    float currentHeightY;
    float heightAligmentSpeed;
    float sineWobble;
};