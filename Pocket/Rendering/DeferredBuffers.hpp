//
//  DeferredBuffers.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/16/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "OpenGL.hpp"

namespace Pocket {
    class DeferredBuffers {
    public:
        DeferredBuffers();
        ~DeferredBuffers();
        
        void Initialize();
        void Destroy();
        void Resize(int width, int height);
        void BeginGeometry();
        void BeginLights();
        void End();
        
        void SaveBuffer(std::string filename);
        
    private:
        GLuint  gbuffer_framebuffer;
        GLuint  gbuffer[2];
        GLuint  depth_buffer;
        
        int width;
        int height;
        
    };
}