//
//  Groundable.h
//  Homeland
//
//  Created by Jeppe Nielsen on 23/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Vector2.hpp"

using namespace Pocket;

struct Groundable {
    float alignmentSpeed;
    std::vector<Vector2> positions;
};