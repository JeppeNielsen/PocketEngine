//
//  Sizeable.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Sizeable.hpp"

using namespace Pocket;

Sizeable::Sizeable() : Size(this) {}

void Sizeable::Reset() {
    Size = 1;
}

void Sizeable::Clone(const Sizeable& source) {
    Size = source.Size;
}

void Sizeable::DebugLog(std::ostringstream &s) {
    s<<"Size = " << Size().x << ", " << Size().y;
}