//
//  CompetitionProJoystick.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/11/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Event.hpp"

namespace Pocket {
    struct CompetitionProJoystick {
        Event<int> ButtonDown;
        Event<int> ButtonUp;
    };
}
