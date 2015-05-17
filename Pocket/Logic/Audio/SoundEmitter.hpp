//
//  SoundEmitter.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponent.hpp"
#include "Property.hpp"


namespace Pocket {
    Component(SoundEmitter)
public:
        SoundEmitter();
    
        Property<SoundEmitter*, float> Volume;
        Property<SoundEmitter*, float> Pitch;
        Property<SoundEmitter*, bool> Looping;
        Property<SoundEmitter*, bool> Playing;
        Property<SoundEmitter*, float> MinDistance;
        Property<SoundEmitter*, float> MaxDistance;
    
        void Emit();
    
        void Reset();
        void Clone(const SoundEmitter& other);
    };
}