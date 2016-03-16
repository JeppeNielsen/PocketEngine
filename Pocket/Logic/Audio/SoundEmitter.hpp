//
//  SoundEmitter.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"


namespace Pocket {
    class SoundEmitter {
public:
        SoundEmitter();
    
        Property<float> Volume;
        Property<float> Pitch;
        Property<bool> Looping;
        Property<bool> Playing;
        Property<float> MinDistance;
        Property<float> MaxDistance;
    
        void Emit();
    };
}