//
//  Label.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Label.hpp"

using namespace Pocket;

Label::Label() :
FontSize(this),
Text(this),
HAlignment(this),
VAlignment(this),
WordWrap(this) {}

void Label::Reset() {
    FontSize = 1.0f;
    Text = "";
    HAlignment = Font::Left;
    VAlignment = Font::Top;
    WordWrap = false;
}

void Label::Clone(const Pocket::Label &source) {
    FontSize = source.FontSize;
    Text = source.Text;
    HAlignment = source.HAlignment;
    VAlignment = source.VAlignment;
    WordWrap = source.WordWrap;
}