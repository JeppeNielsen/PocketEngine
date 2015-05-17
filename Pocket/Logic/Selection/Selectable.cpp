//
//  Selectable.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Selectable.hpp"

using namespace Pocket;

Selectable::Selectable() : Selected(this) {
    
}

void Selectable::Reset() {
    Selected = false;
}

void Selectable::Clone(const Pocket::Selectable &other) {
    Selected = other.Selected;
}
