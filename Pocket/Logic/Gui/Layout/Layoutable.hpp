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
    
    enum class HAlignment { None, Left, Center, Right, Relative };
    enum class VAlignment { None, Top, Center, Bottom, Relative };
    
    Property<Layoutable*, HAlignment> HorizontalAlignment;
    Property<Layoutable*, VAlignment> VerticalAlignment;
    
    void Reset();
    void Clone(const Layoutable& other);
    
    };
}
