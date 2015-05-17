//
//  MenuAnimator.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include "GameComponent.hpp"

namespace Pocket {
    Component(MenuAnimator)
    public:
        void Reset();
        std::string Menu;
        std::string FadeInAnimation;
        std::string FadeOutAnimation;
    
    };
}