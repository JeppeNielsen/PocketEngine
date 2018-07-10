//
//  WindowWeb.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Window.hpp"
#include "InputDevice.hpp"
#include "OpenGL.hpp"

namespace Pocket {
	class WindowRaspberryPi : public Pocket::Window
	{
    public:
        void Create(int width, int height, bool fullScreen);
        void Destroy();
        bool Update();
        void PreRender();
        void PostRender();
        
    private:
        InputDevice& inputDevice();
        int GetMouseButtonIndex(int index);
        Vector2 ConvertMousePosition(int x, int y);
    };
}
