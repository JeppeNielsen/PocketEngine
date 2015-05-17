//
//  Sizeable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include <string>
#include "Vector2.hpp"
#include "GameComponent.hpp"

namespace Pocket {
    Component(Sprite)
    public:
        Sprite();
        Property<Sprite*, std::string> SpriteName;
        Property<Sprite*, Vector2> CornerSize;
        void Reset();
    protected:
        void Clone(const Sprite& source);
    };
}