//
//  TextBox.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include <string>
#include "TypeInfo.hpp"

namespace Pocket {
    struct TextBox {
        TextBox() {
            Text = "";
            Active = false;
            DeselectOnEnter = true;
        }
        
        Property<std::string> Text;
        Property<bool> Active;
        Property<bool> DeselectOnEnter;
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(Text);
        TYPE_FIELD(Active);
        TYPE_FIELDS_END
    };
}