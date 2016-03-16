//
//  Colorable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/27/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once

#include "Property.hpp"
#include "Colour.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    class Colorable {
    public:
        Colorable();
        Property<Colour> Color;
        
        TYPE_FIELDS_BEGIN;
        TYPE_FIELD(Color);
        TYPE_FIELDS_END;
    };
}