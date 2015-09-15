//
//  Label.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include <string>
#include "Property.hpp"
#include "Font.hpp"
#include "GameComponent.hpp"

namespace Pocket {
    Component(Label)
    public:
        
        Label();
		Property<Label*, float> FontSize;
		Property<Label*, std::string> Text;
		Property<Label*, Font::HAlignment> HAlignment;
		Property<Label*, Font::VAlignment> VAlignment;
		Property<Label*, bool> WordWrap;
        
        void Reset();
        void Clone(const Label& source);
    
        SERIALIZE_FIELDS_BEGIN
        SERIALIZE_FIELD(FontSize);
        SERIALIZE_FIELD(Text);
        SERIALIZE_FIELD(HAlignment);
        SERIALIZE_FIELD(VAlignment);
        SERIALIZE_FIELD(WordWrap);
        SERIALIZE_FIELDS_END
    };
}