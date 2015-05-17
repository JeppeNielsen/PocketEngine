//
//  Material.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/23/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Material.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace Pocket;

Material::Material()
:
LightingEnabled(this), BlendMode(this), DepthEnabled(this), Mask(this)
{
}

Material::~Material() {}

void Material::Reset() {
    LightingEnabled = false;
    BlendMode = BlendMode::None;
    DepthEnabled = true;
    Clip = 0;
    Mask = 0;
}

void Material::Clone(const Pocket::Material &source) {
    LightingEnabled = source.LightingEnabled;
    BlendMode = source.BlendMode;
    DepthEnabled = source.DepthEnabled;
    Clip = source.Clip;
    Mask = source.Mask;
}
