//
//  TransformAnimatorSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "TransformAnimator.hpp"

namespace Pocket {
    class TransformAnimatorSystem : public GameSystem {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        
        void Update(float dt);
    private:
        
    };
}