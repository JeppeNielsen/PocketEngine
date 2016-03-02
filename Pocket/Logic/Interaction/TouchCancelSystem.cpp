//
//  TouchCancelSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/11/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TouchCancelSystem.hpp"
#include "InputManager.hpp"

using namespace Pocket;

void TouchCancelSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Touchable>()->Down.Bind(this, &TouchCancelSystem::TouchableDown, object);
    object->GetComponent<Touchable>()->Up.Bind(this, &TouchCancelSystem::TouchableUp, object);
}

void TouchCancelSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Touchable>()->Down.Unbind(this, &TouchCancelSystem::TouchableDown, object);
    object->GetComponent<Touchable>()->Up.Unbind(this, &TouchCancelSystem::TouchableUp, object);
}

void TouchCancelSystem::Update(float dt) {
    for (typename ActiveTouchables::iterator it = activeTouchables.begin(); it!=activeTouchables.end(); ++it) {
        
        Vector3 worldPosition =
            !it->second.canceller->trackTouchPosition ?
            it->second.transform->World().TransformPosition(it->second.transform->Anchor) :
            Vector3(it->second.touch.Input->GetTouchPosition(it->second.touch.Index),0);
        
        Vector3 delta = worldPosition - it->second.prevPosition;
        it->second.prevPosition = worldPosition;
    
        it->second.distanceTravelled += delta.Length();
        std::cout<<it->second.distanceTravelled<<std::endl;

        if (it->second.distanceTravelled>it->second.canceller->MovementToCancel) {
            it->second.touchable->Cancel();
        }
    }
}

void TouchCancelSystem::TouchableDown(TouchData touch, GameObject *object) {
    Transform* transform = object->GetComponent<Transform>();
    Vector3 startPosition = !object->GetComponent<TouchableCanceller>()->trackTouchPosition ?
    transform->World().TransformPosition(transform->Anchor)
    :
    Vector3(touch.Position,0);
    activeTouchables[touch.Touchable] = {
        object->GetComponent<Touchable>(),
        transform,
        object->GetComponent<TouchableCanceller>(), 0, startPosition, touch
    };
}

void TouchCancelSystem::TouchableUp(TouchData touch, GameObject *object) {
    typename ActiveTouchables::iterator it = activeTouchables.find(touch.Touchable);
    if (it!=activeTouchables.end()) {
        activeTouchables.erase(it);
    }
}