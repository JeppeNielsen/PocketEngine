//
//  GameManager.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 9/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "GameManager.hpp"


using namespace Pocket;

Vector2 GameManager::ScreenSize() const {
    return screenSize;
}

Box GameManager::Viewport() const {
    return Box(0, screenSize.y, screenSize.x, 0);
}