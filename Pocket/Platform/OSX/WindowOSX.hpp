//
//  WindowOSX.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/6/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "OSXWindowCreator.h"
#include "Window.hpp"
#include "InputManager.hpp"
#include "OpenGL.hpp"
#include <map>

namespace Pocket {
	class WindowOSX : public Pocket::Window
	{
    public:
        void Create(int width, int height, bool fullScreen);
        void Destroy();
        bool Update();
        void PreRender();
        void PostRender();
        void Begin();
        
    private:
        void Loop(bool exit);
        void osxWindowCreated(void* window);
        void CalcViewportSize();
        
        GLuint frameBuffer;
        
        GLint backingWidth;
        GLint backingHeight;
        
        GLuint depthRenderbuffer;
        GLuint viewRenderbuffer;
        GLuint viewFramebuffer;
        
        void* openGLContext;
        void* win;
        int backingPixelWidth;
        int backingPixelHeight;
	};
}