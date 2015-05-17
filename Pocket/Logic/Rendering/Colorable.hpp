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
#include "GameComponent.hpp"

namespace Pocket {
    Component(Colorable)
    public:
        Colorable();
        Property<Colorable*, Colour> Color;
        void Reset();
    
        void Serialize(ISerializedProperty* property);
        void Deserialize(ISerializedProperty* property);
    
    };
}