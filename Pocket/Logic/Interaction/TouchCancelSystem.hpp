//
//  TouchCancelSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/11/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include <map>
#include "Touchable.hpp"
#include "TouchableCanceller.hpp"
#include "Transform.hpp"

namespace Pocket {
    class TouchCancelSystem : public GameSystem {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
    private:
        
        struct ActiveTouchable {
            Touchable* touchable;
            Transform* transform;
            TouchableCanceller* canceller;
            float distanceTravelled;
            Vector3 lastWorldPosition;
        };
        
        typedef std::map<Touchable*, ActiveTouchable> ActiveTouchables;
        ActiveTouchables activeTouchables;
        
        void TouchableDown(TouchData touch, GameObject* object);
        void TouchableUp(TouchData touch, GameObject* object);
        
    };
}