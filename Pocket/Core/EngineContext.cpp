//
//  EngineContext.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 16/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EngineContext.hpp"

using namespace Pocket;

Rect EngineContext::Viewport() const {
    return Rect(0, ScreenSize());
}