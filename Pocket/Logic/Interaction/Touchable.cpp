//
//  Touchable.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/16/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Touchable.hpp"

using namespace Pocket;

Touchable::Touchable()
{
	
}

Touchable::~Touchable() { }

void Touchable::Reset() {
    ClickThrough = false;
}

void Touchable::Clone(const Touchable& source) {
    ClickThrough = source.ClickThrough;
}

void Touchable::Cancel() {
    Cancelled(this);
}