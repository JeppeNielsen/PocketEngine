//
//  SpringTransformSystem.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/4/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Spring.h"

using namespace Pocket;

class SpringTransformSystem : public GameSystem<Transform, Spring> {
public:
    void Update(float dt);
};