//
//  Selectable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponent.hpp"
#include "Property.hpp"

namespace Pocket {
    Component(Selectable)
public:
        Selectable();
        void Reset();
        void Clone(const Selectable& other);
        Property<Selectable*, bool> Selected;
    };
}