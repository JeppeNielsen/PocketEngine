//
//  Menu.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include <string>

namespace Pocket {
    struct Menu {
        Menu();
        Property<std::string> ActiveMenu;
    };
}