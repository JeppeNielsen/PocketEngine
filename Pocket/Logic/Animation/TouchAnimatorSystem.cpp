//
//  TouchAnimatorSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/9/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TouchAnimatorSystem.hpp"

using namespace Pocket;

void TouchAnimatorSystem::Initialize() {
    AddComponent<TouchAnimator>();
    AddComponent<Touchable>();
    AddComponent<TransformAnimator>();
    AddComponent<TransformAnimationDatabase>();
}


void TouchAnimatorSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Down += event_handler(this, &TouchAnimatorSystem::Down, object);
    object->GetComponent<Touchable>()->Up += event_handler(this, &TouchAnimatorSystem::Up, object);
}

void TouchAnimatorSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Down -= event_handler(this, &TouchAnimatorSystem::Down, object);
    object->GetComponent<Touchable>()->Up -= event_handler(this, &TouchAnimatorSystem::Up, object);
}

void TouchAnimatorSystem::Down(Pocket::TouchData d, Pocket::GameObject *object) {
    TransformAnimation* anim = object->GetComponent<TransformAnimationDatabase>()->GetAnimation(object->GetComponent<TouchAnimator>()->DownAnimation);
    object->GetComponent<TransformAnimator>()->Play(anim, 0.2f);
}

void TouchAnimatorSystem::Up(Pocket::TouchData d, Pocket::GameObject *object) {
    TransformAnimation* anim = object->GetComponent<TransformAnimationDatabase>()->GetAnimation(object->GetComponent<TouchAnimator>()->UpAnimation);
    object->GetComponent<TransformAnimator>()->Play(anim, 0.2f);
}