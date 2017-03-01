//
//  TriggerTouchSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Trigger.hpp"
#include "Touchable.hpp"

namespace Pocket {
    class TriggerTouchSystem : public GameSystem<Trigger, Touchable> {
    protected:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
    private:
        void Clicked(TouchData d, GameObject* object);
    };
}
