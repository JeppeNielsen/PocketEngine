//
//  Layoutable.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/19/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Layoutable.hpp"

using namespace Pocket;

Layoutable::Layoutable() {
    HorizontalAlignment = HAlignment::None;
    VerticalAlignment = VAlignment::None;
    ChildLayouting = ChildLayouting::None;
}
