//
//  LineMesh.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 11/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "Vector3.hpp"
#include "Property.hpp"

namespace Pocket {
    struct LineRenderer {
        LineRenderer();
        std::vector<Vector3> vertices;
        Property<float> Width;
    };
}
