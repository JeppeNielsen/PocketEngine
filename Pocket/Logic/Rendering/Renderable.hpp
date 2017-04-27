//
//  Renderable.hpp
//  Shaders
//
//  Created by Jeppe Nielsen on 25/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "TypeInfo.hpp"
#include "Camera.hpp"

namespace Pocket {

    enum class BlendModeType {
        Opaque,
        Alpha,
        Add
    };

    struct Renderable {
    public:
        Renderable() { BlendMode = BlendModeType::Opaque; Mask = 0; Clip = 0; }
        Property<BlendModeType> BlendMode;
        Property<RenderMask> Mask;
        int Clip;
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(BlendMode)
        TYPE_FIELD(Mask)
        TYPE_FIELDS_END
    };
}
