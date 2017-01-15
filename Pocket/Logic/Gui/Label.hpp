//
//  DynamicLabel.hpp
//  TestFreeType
//
//  Created by Jeppe Nielsen on 13/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "Property.hpp"
#include "Font.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    struct Label {
    public:
        Label() {
            FontSize = 1;
            Text = "";
            HAlignment = Font::HAlignment::Left;
            VAlignment = Font::VAlignment::Top;
            WordWrap = false;
        }
        
        Property<float> FontSize;
        Property<std::string> Text;
        Property<Font::HAlignment> HAlignment;
        Property<Font::VAlignment> VAlignment;
        Property<bool> WordWrap;
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(FontSize);
        TYPE_FIELD(Text);
        TYPE_FIELD(HAlignment);
        TYPE_FIELD(VAlignment);
        TYPE_FIELD(WordWrap);
        TYPE_FIELDS_END
    };
}
