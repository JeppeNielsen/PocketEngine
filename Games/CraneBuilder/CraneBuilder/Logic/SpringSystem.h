//
//  SpringSystem.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/4/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Spring.h"

using namespace Pocket;

class SpringSystem : public GameSystem<Spring> {
public:
    void Simulate(float dt);
    void ResetTension();
    void CheckForBrokenSprings();
};