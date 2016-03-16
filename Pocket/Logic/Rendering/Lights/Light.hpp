//
//  Light.hpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/26/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "Property.hpp"

namespace Pocket {
    struct Light {
    public:
        Light();
    
        enum class LightType {
            Directional,
            Point
        };
        
        Property<LightType> Type;
        Property<float> Brightness;
        Property<float> Radius;
    };
}