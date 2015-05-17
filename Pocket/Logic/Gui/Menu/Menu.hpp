//
//  Menu.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "GameComponent.hpp"
#include <string>

namespace Pocket {
    Component(Menu)
    public:

        Menu();
        Property<Menu*, std::string> ActiveMenu;
        void Reset();
    };

}