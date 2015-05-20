//
//  Groundable.cpp
//  Platformer
//
//  Created by Jeppe Nielsen on 29/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Groundable.hpp"

Groundable::Groundable() : IsGrounded(this) { }

void Groundable::Reset() {
    IsGrounded = false;
}