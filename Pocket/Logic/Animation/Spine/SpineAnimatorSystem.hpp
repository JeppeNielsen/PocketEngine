//
//  SpineAnimatorSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 05/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "SpineAnimator.hpp"
#include "SpineModel.hpp"
#include "SpineSkeleton.hpp"

namespace Pocket {
    class SpineAnimatorSystem : public GameSystem {
    public:
        void Initialize();
        void Update(float dt);
    };
}