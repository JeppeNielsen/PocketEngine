//
//  Sizeable.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Sprite.hpp"

using namespace Pocket;

Sprite::Sprite() : SpriteName(this), CornerSize(this) {}

void Sprite::Reset() {
    SpriteName = "";
    CornerSize = 0;
}

void Sprite::Clone(const Sprite& source) {
    SpriteName = source.SpriteName;
    CornerSize = source.CornerSize;
}
