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

namespace Pocket {
    struct Sprite {
    public:
        Sprite() {
             SpriteName = "";
            CornerSize = 0;
        }
        Property<std::string> SpriteName;
        Property<Vector2> CornerSize;
    
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(SpriteName);
        TYPE_FIELD(CornerSize);
        TYPE_FIELDS_END
    };
}