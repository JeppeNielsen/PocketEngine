//
//  Animator.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 14/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "TypeInfo.hpp"

namespace Pocket {
    class Animator {
    public:
        Property<float> Time;
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(Time)
        TYPE_FIELDS_END
    };
}
