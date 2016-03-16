//
//  MenuButton.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Menu.hpp"
#include <string>

namespace Pocket {
    struct MenuButton {
        MenuButton();
        Menu* menu;
        std::string menuToActivate;
    };
}