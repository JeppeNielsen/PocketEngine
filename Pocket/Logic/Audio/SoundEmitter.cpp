//
//  SoundEmitter.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "SoundEmitter.hpp"

using namespace Pocket;

SoundEmitter::SoundEmitter() :
Volume(this),Pitch(this),Looping(this), Playing(this), MinDistance(this), MaxDistance(this) {}

void SoundEmitter::Reset() {
    Volume = 1.0;
    Pitch = 1.0;
    Looping = true;
    Playing = true;
    MinDistance = 3.0f;
    MaxDistance = 20.0f;
}

void SoundEmitter::Clone(const Pocket::SoundEmitter &other) {
    Volume = other.Volume;
    Pitch = other.Pitch;
    Looping = other.Looping;
    Playing = other.Playing;
    MinDistance = other.MinDistance;
    MaxDistance = other.MaxDistance;
}

void SoundEmitter::Emit() {
    Playing = false;
    Playing = true;
}