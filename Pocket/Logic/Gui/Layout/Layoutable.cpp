//
//  Layoutable.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/19/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Layoutable.hpp"

using namespace Pocket;

Layoutable::Layoutable() : HorizontalAlignment(this), VerticalAlignment(this), ChildLayouting(this) {}

void Layoutable::Reset() {
    HorizontalAlignment = HAlignment::None;
    VerticalAlignment = VAlignment::None;
    ChildLayouting = ChildLayouting::None;
}

void Layoutable::Clone(const Pocket::Layoutable &other) {
    HorizontalAlignment = other.HorizontalAlignment;
    VerticalAlignment = other.VerticalAlignment;
}

