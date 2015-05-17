//
//  Material.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/23/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameComponent.hpp"
#include "Property.hpp"
#include <map>
#include "OpenGL.hpp"
#include "Colour.hpp"
#include "Camera.hpp"

namespace Pocket {
    
    struct BlendMode {
        enum Mode {
            None,
            Alpha,
            Add
        };
    };
    
    Component(Material)
    public:
        Material();
        ~Material();
    
        Property<Material*, bool> LightingEnabled;
        Property<Material*, BlendMode::Mode> BlendMode;
        Property<Material*, bool> DepthEnabled;
        Property<Material*, RenderMask> Mask;
    
        int Clip; // 0 == no clipping, 1 == push clip, 2 == pop clip
    
        void Reset();
    
    protected:
        void Clone(const Material& source);
     
    };
}