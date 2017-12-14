//
//  CompetitionProJoystickSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/11/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "CompetitionProJoystick.hpp"
#include "HidDevice.hpp"

namespace Pocket {
    class CompetitionProJoystickSystem : public GameSystem<HidDevice, CompetitionProJoystick> {
    public:
        void Update(float dt);
    };
}

