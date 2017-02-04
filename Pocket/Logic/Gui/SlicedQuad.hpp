//
//  Quad.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "TypeInfo.hpp"
#include "Property.hpp"
#include "Rect.hpp"

namespace Pocket {
    struct SlicedQuad {
    public:
        SlicedQuad() {
            OuterSize = 0;
            InnerSize = 0;
        }
        Property<float> OuterSize;
        Property<float> InnerSize;
    
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(OuterSize);
        TYPE_FIELD(InnerSize);
        TYPE_FIELDS_END
    };
}
