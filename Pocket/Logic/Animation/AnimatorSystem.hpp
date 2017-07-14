//
//  AnimatorSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 14/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Animation.hpp"
#include "Animator.hpp"

namespace Pocket {
    class AnimatorSystem : public GameSystem<Animation, Animator> {
    protected:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
    };
}
