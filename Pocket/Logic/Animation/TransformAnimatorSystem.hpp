//
//  TransformAnimatorSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "TransformAnimator.hpp"

namespace Pocket {
    class TransformAnimatorSystem : public GameSystem<TransformAnimator, Transform> {
    public:
        void Initialize();
        void Update(float dt);
    };
}