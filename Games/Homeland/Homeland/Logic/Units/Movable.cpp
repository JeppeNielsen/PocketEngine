//
//  Movable.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 23/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Movable.h"

Movable::Movable() : Target(this) {}

void Movable::Reset() {
    Speed = 0.2f;
    path.clear();
}