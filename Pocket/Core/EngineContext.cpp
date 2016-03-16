//
//  EngineContext.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 16/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EngineContext.hpp"

using namespace Pocket;

Box EngineContext::Viewport() const {
    return Box(0, ScreenSize().y, ScreenSize().x, 0);
}