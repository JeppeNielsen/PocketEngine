//
//  Orderable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/29/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Property.hpp"

namespace Pocket {
    Component(Orderable)
    public:
        Orderable();
        Property<Orderable*, int> Order;
        void Reset();
        void Clone(const Orderable& source);
    
    };
}