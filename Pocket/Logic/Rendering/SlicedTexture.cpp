//
//  SliceableTexture.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "SlicedTexture.hpp"

using namespace Pocket;

SlicedTexture::SlicedTexture() {
    BaseArea = Box(0, 0, 1, 1);
    OuterArea = Box(0, 0, 1, 1);
    InnerArea = Box(0, 0, 1, 1);
}
