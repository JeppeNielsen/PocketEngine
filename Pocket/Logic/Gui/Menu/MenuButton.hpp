//
//  MenuButton.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameComponent.hpp"
#include "Menu.hpp"
#include <string>

namespace Pocket {
    
    Component(MenuButton)
    public:
        void Reset();
        Menu* menu;
        std::string menuToActivate;

    };
}