//
//  TouchAnimator.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/9/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include <string>

namespace Pocket {
    struct TouchAnimator {
        std::string DownAnimation;
        std::string UpAnimation;
    };
}