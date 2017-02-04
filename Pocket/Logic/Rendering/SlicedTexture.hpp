//
//  SliceableTexture.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "TypeInfo.hpp"
#include "Property.hpp"
#include "Box.hpp"

namespace Pocket {
    struct SlicedTexture {
    public:
        SlicedTexture();
        
        Property<Box> BaseArea;
        Property<Box> OuterArea;
        Property<Box> InnerArea;
    
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(BaseArea);
        TYPE_FIELD(OuterArea);
        TYPE_FIELD(InnerArea);
        TYPE_FIELDS_END
    };
}
