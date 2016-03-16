//
//  Light.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/26/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Light.hpp"

using namespace Pocket;

Light::Light() {
    Type = LightType::Directional;
    Brightness = 1.0f;
    Radius = 10;
}
