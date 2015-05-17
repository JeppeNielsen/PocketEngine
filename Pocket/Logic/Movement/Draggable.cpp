//
//  Dragger.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Draggable.hpp"
using namespace Pocket;

Draggable::Draggable() : IsDragging(this) { }

void Draggable::Reset() {
    Movement = MovementMode::Normal;
    IsDragging = false;
}

void Draggable::Clone(const Pocket::Draggable &other) {
    Movement = other.Movement;
    IsDragging = other.IsDragging;
}

