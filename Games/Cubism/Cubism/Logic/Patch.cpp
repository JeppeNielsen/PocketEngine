//
//  Patch.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Patch.hpp"

Patch::Patch() : BlockAttached(this) {}
Patch::~Patch() {}

void Patch::Reset() {
    BlockAttached = 0;
}