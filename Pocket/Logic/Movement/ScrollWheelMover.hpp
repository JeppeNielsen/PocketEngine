//
//  ScrollWheelMover.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "Vector3.hpp"

namespace Pocket {
    class ScrollWheelMover {
    public:
        Property<float> Speed;
        Property<Vector3> Movement;
    };
}
