//
//  RenderTexture.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once


namespace Pocket {
    class RenderTexture {
    private:
        using GLuint = unsigned int;
        using GLint = int;
        
        GLuint framebuffer;
        GLuint texure;
        GLint old_fbo;
        
    public:
        RenderTexture();
        ~RenderTexture();
    
        void Initialize(int width, int height);
        void Free();
        void Begin();
        void End();
        GLuint GetTexture() const;
    };
}
