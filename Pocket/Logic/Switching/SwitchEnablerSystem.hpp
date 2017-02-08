//
//  SwitchEnablerSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 22/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Switch.hpp"
#include "SwitchEnabler.hpp"

namespace Pocket {
    class SwitchEnablerSystem : public GameSystem<Switch, SwitchEnabler> {
    protected:
        void ObjectAdded(GameObject* object) override;
        void ObjectRemoved(GameObject* object) override;
    private:
        void SwitchChanged(GameObject* object);
    };
}
