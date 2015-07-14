//
//  ShaderCollection.h
//  Shaders
//
//  Created by Jeppe Nielsen on 10/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "Shader.hpp"

namespace Pocket {
    class ShaderCollection {
        public:
            void Initialize();
            
            Shader<Vertex> Colored;
            Shader<Vertex> Textured;
        
            Shader<Vertex> LitColored;
            Shader<Vertex> LitTextured;
    };
}