//
//  Light.hpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/26/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameComponent.hpp"
#include "Property.hpp"
#include "Colour.hpp"

namespace Pocket {
    Component(Light)
    public:
        Light();
    
        enum class LightType {
            Directional,
            Point
        };
        
        Property<Light*, LightType> Type;
        Property<Light*, float> Brightness;
        Property<Light*, float> Radius;
    
        void Reset();
    protected:
    
        void Clone(const Light& source);
    };
}