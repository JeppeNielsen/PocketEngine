//
//  SwitchSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Switch.hpp"
#include <vector>

namespace Pocket {
    class SwitchSystem : public GameSystem<Switch> {
    protected:
        void Initialize() override;
        void ObjectAdded(GameObject* object) override;
        void ObjectRemoved(GameObject* object) override;
        void Update(float dt) override;
    private:
        void SwitchChanged(GameObject* object);
        bool isDirty;
        bool disableListening;
        std::vector<Switch*> turnedOnSwitches;
    };
}
