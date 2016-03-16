//
//  Sizeable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Vector2.hpp"
#include "TypeInfo.hpp"
#include "Property.hpp"

namespace Pocket {
    struct Sizeable {
    public:
        Sizeable() {
            Size = 1.0f;
        }
        
        Property<Vector2> Size;
    
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(Size)
        TYPE_FIELDS_END
    };
}