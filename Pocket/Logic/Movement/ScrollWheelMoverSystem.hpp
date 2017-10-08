//
//  ScrollWheelMoverSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "ScrollWheelMover.hpp"
#include "Hoverable.hpp"

namespace Pocket {
    class ScrollWheelMoverSystem : public GameSystem<Transform, ScrollWheelMover, Hoverable> {
        public:
            void Initialize() override;
            void Destroy() override;
            void ObjectAdded(GameObject* object) override;
            void ObjectRemoved(GameObject* object) override;
            void Update(float dt) override;
        
        private:
            void Enter(TouchData d, GameObject* object);
            void Exit(TouchData d, GameObject* object);
        
            void ScrollChanged(float delta);
        
            float currentScrollValue;
        
            ObjectCollection activeObjects;
        
    };
}


