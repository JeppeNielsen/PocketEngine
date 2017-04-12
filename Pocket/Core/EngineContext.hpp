//
//  GameManager.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 9/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "Vector2.hpp"
#include "Rect.hpp"
#include "AppMenu.hpp"
#include "InputDevice.hpp"

namespace Pocket {
    class EngineContext {
    public:
        EngineContext();
        ~EngineContext();
        Property<Vector2> ScreenSize;
        Rect Viewport() const;
        AppMenu& Menu();
        InputDevice& InputDevice();
        float ScreenScalingFactor;
    private:
        AppMenu* menu;
        class InputDevice inputDevice;
    };
}