//
//  TouchCancelSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/11/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameSystem.hpp"
#include <map>
#include "Touchable.hpp"
#include "TouchableCanceller.hpp"
#include "Transform.hpp"

namespace Pocket {

    template<typename T>
    class TouchCancelSystem : public GameSystem<T, Touchable, TouchableCanceller, Transform> {
    public:
    
        void ObjectAdded(GameObject<T> *object) {
            object->template GetComponent<Touchable>()->Down += event_handler(this, &TouchCancelSystem::TouchableDown, object);
            object->template GetComponent<Touchable>()->Up += event_handler(this, &TouchCancelSystem::TouchableUp, object);
        }

        void ObjectRemoved(GameObject<T> *object) {
            object->template GetComponent<Touchable>()->Down -= event_handler(this, &TouchCancelSystem::TouchableDown, object);
            object->template GetComponent<Touchable>()->Up -= event_handler(this, &TouchCancelSystem::TouchableUp, object);
        }
        
        void Update(float dt) {
            for (typename ActiveTouchables::iterator it = activeTouchables.begin(); it!=activeTouchables.end(); ++it) {
                
                Vector3 worldPosition =
                    !it->second.canceller->trackTouchPosition ?
                    it->second.transform->World.GetValue()->TransformPosition(it->second.transform->Anchor) :
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
        
    private:
        
        struct ActiveTouchable {
            Touchable* touchable;
            Transform* transform;
            TouchableCanceller* canceller;
            float distanceTravelled;
            Vector3 prevPosition;
            TouchData touch;
        };
        
        typedef std::map<Touchable*, ActiveTouchable> ActiveTouchables;
        ActiveTouchables activeTouchables;
        
        void TouchableDown(TouchData touch, GameObject<T> *object) {
            Transform* transform = object->template GetComponent<Transform>();
            Vector3 startPosition = !object->template GetComponent<TouchableCanceller>()->trackTouchPosition ?
            transform->World.GetValue()->TransformPosition(transform->Anchor)
            :
            Vector3(touch.Position,0);
            activeTouchables[touch.Touchable] = {
                object->template GetComponent<Touchable>(),
                transform,
                object->template GetComponent<TouchableCanceller>(), 0, startPosition, touch
            };
        }

        void TouchableUp(TouchData touch, GameObject<T> *object) {
            typename ActiveTouchables::iterator it = activeTouchables.find(touch.Touchable);
            if (it!=activeTouchables.end()) {
                activeTouchables.erase(it);
            }
        }
    };
}