//
//  LimitableSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Limitable.hpp"

namespace Pocket {
    class LimitableSystem : public GameSystem<Transform, Limitable> {
    public:
        void Update(float dt);
    };
}