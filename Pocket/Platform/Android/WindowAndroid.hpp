//
//  WindowAndroid.h
//  Android
//
//  Created by Jeppe Nielsen on 03/07/15.
//
//

#pragma once

#include "Window.hpp"
#include "InputDevice.hpp"
#include "OpenGL.hpp"

int main();

namespace Pocket {
	class WindowAndroid : public Pocket::Window
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
    };
}