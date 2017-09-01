//
//  GameWorldViewport.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "RenderTexture.hpp"
#include "Point.hpp"
#include "InputDevice.hpp"

namespace Pocket {
    class GameWorld;
    class GameWorldViewport {
    public:
        GameWorldViewport();
        GameWorld* World;
        Point RenderTextureSize;
        RenderTexture renderTexture;
        InputDevice inputDevice;
    };
}
