//
//  TouchSwitchSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Switch.hpp"
#include "Touchable.hpp"

namespace Pocket {
    class TouchSwitchSystem : public GameSystem<Switch, Touchable> {
    protected:
        void ObjectAdded(GameObject* object) override;
        void ObjectRemoved(GameObject* object) override;
    private:
        void TouchClick(TouchData d, GameObject* object);
    };
}
