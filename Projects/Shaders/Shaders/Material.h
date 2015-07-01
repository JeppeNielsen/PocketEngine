//
//  Material.h
//  Shaders
//
//  Created by Jeppe Nielsen on 25/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Shader.H"
#include "Camera.hpp"

using namespace Pocket;

enum class BlendMode {
    Opaque,
    Alpha,
    Add
};

Component(Material)
public:
    Material() : Shader(this), BlendMode(this), Mask(this), Clip(0) { }
    Property<Material*, IShader*> Shader;
    Property<Material*, BlendMode> BlendMode;
    Property<Material*, RenderMask> Mask;
    int Clip; // 0 == no clipping, 1 == push clip, 2 == pop clip

    void Reset() {
        Shader = 0;
        BlendMode = BlendMode::Opaque;
        Mask = 0;
        Clip = 0;
    }
};