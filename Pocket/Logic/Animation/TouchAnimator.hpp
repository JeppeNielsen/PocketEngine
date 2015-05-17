//
//  TouchAnimator.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/9/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponent.hpp"
#include <string>

namespace Pocket {
    Component(TouchAnimator)
public:
        void Reset();
        std::string DownAnimation;
        std::string UpAnimation;
    
    };
}