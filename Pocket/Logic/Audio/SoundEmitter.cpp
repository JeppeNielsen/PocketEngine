//
//  SoundEmitter.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "SoundEmitter.hpp"

using namespace Pocket;

SoundEmitter::SoundEmitter() {
    Volume = 1.0;
    Pitch = 1.0;
    Looping = true;
    Playing = true;
    MinDistance = 3.0f;
    MaxDistance = 20.0f;
}

void SoundEmitter::Emit() {
    Playing = false;
    Playing = true;
}