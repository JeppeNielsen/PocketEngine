//
//  IOSWindow.h
//  Pocket
//
//  Created by Jeppe Nielsen on 3/10/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "IOSWindowCreator.h"
#include "Window.hpp"
#include "InputManager.hpp"
#include "OpenGL.hpp"
#include <map>

namespace Pocket {
	class WindowiOS : public Pocket::Window
	{
    public:
        void Create(int width, int height, bool landscape);
        void Destroy();
        bool Update(IInputManagerIterator* inputManagers);
        void PreRender();
        void PostRender();
        void Begin();
        
    private:
        void Loop(bool exit);
        void iosWindowCreated(void* window);
        void KeyboardChanged(InputDevice::KeyboardEventData keyboardData);
        /*
        void* context;
        GLuint framebuffer;
        GLuint _colorRenderBuffer;
        */
        
        GLuint frameBuffer;
        
        GLint backingWidth;
        GLint backingHeight;
        
        GLuint depthRenderbuffer;
        GLuint viewRenderbuffer;
        GLuint viewFramebuffer;
        
        void* context;
        void* window;
        
        
	};
}