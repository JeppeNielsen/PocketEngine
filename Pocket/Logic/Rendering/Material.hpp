//
//  Material.h
//  Shaders
//
//  Created by Jeppe Nielsen on 25/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "TypeInfo.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

namespace Pocket {

    enum class BlendModeType {
        Opaque,
        Alpha,
        Add
    };

    struct Material {
    public:
        Material() : Shader(this), BlendMode(this), Mask(this), Clip(0) { }
        Property<Material*, IShader*> Shader;
        Property<Material*, BlendModeType> BlendMode;
        Property<Material*, RenderMask> Mask;
        int Clip;
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(BlendMode)
        TYPE_FIELD(Mask)
        TYPE_FIELDS_END
    };
}