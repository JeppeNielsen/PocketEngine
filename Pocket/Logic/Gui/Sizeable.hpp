//
//  Sizeable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Vector2.hpp"
#include "Property.hpp"
#include "GameComponent.hpp"

namespace Pocket {
    Component(Sizeable) 
    public:
        Sizeable();
        Property<Sizeable*, Vector2> Size;
        void Reset();
    protected:
        void Clone(const Sizeable& source);
        void DebugLog(std::ostringstream& s);
    };
}