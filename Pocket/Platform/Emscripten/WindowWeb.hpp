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
#include "SDL/SDL.h"

namespace Pocket {
	class WindowWeb : public Pocket::Window
	{
    public:
        void Create(int width, int height, bool fullScreen);
        void Destroy();
        bool Update(IInputManagerIterator* inputManagers);
        void PreRender();
        void PostRender();
        void Begin();
        
        void Loop();
        
    private:
        int frameRate;
        bool active;
        bool paused;
		int previousTime;
        InputDevice inputDevice;
        int GetMouseButtonIndex(int index);
        Vector2 ConvertMousePosition(int x, int y);
        void handleEvent (SDL_Event &event);
    };
}