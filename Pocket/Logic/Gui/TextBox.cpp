//
//  TextBox.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TextBox.hpp"

using namespace Pocket;

TextBox::TextBox() : Text(this), Active(this) {
}

void TextBox::Reset() {
    Text = "";
    Active = false;
}


