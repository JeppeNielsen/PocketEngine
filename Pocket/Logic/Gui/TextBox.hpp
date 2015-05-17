//
//  TextBox.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponent.hpp"
#include "Property.hpp"
#include <string>

namespace Pocket {
    Component(TextBox)
public:
    TextBox();
    void Reset();
    
    Property<TextBox*, std::string> Text;
    Property<TextBox*, bool> Active;
    
    };
}