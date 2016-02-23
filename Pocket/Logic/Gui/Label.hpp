//
//  Label.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include <string>
#include "TypeInfo.hpp"
#include "Property.hpp"
#include "Font.hpp"

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