//
//  Layoutable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/19/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameComponent.hpp"
#include "Property.hpp"
#include "Vector2.hpp"

namespace Pocket {
    Component(Layoutable)
public:
    Layoutable();
    
    struct HAlignment { enum Type { None, Left, Center, Right, Relative }; };
    struct VAlignment { enum Type { None, Top, Center, Bottom, Relative }; };
    
    Property<Layoutable*, HAlignment::Type> HorizontalAlignment;
    Property<Layoutable*, VAlignment::Type> VerticalAlignment;
    
    void Reset();
    void Clone(const Layoutable& other);
    
    };
}
