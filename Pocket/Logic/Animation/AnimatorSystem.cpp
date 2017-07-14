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
        o->GetComponent<Animator>()->Time += dt * 0.1f;
        o->GetComponent<Animation>()->Apply(o, fmodf(o->GetComponent<Animator>()->Time, 2.0f));
    }
}
