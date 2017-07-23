//
//  AnimatorSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 14/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "AnimatorSystem.hpp"

using namespace Pocket;

void AnimatorSystem::ObjectAdded(GameObject *object) {

}

void AnimatorSystem::ObjectRemoved(GameObject *object) {

}

void AnimatorSystem::Update(float dt) {
    for(auto o : Objects()) {
        Animator* animator = o->GetComponent<Animator>();
        Animation* animation = o->GetComponent<Animation>();
        animator->Time += dt;
        animation->Apply(o, animator->Time);
    }
}
