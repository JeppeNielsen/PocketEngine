//
//  Selectable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"

namespace Pocket {
    class Selectable {
    public:
        Selectable();
        Property<bool> Selected;
    };
}