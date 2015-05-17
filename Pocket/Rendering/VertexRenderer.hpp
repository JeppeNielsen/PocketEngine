//
//  RenderCall.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/22/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "OpenGL.hpp"
#include <vector>
#include "Vertex.hpp"



namespace Pocket {
    class Shader;
    class VisibleObject;
    class VertexRenderer {
    public:
        VertexRenderer();
        ~VertexRenderer();
        
        VisibleObject* visibleObject;
        Shader* shader;
        
        const static unsigned int MAX_VERTICES = 30000;
        const static unsigned int MAX_TRIANGLES = (MAX_VERTICES / 4) * 6;
        
        void Init();
        
        void Begin(const size_t& vertexCount, const size_t& trianglesCount);
        
        size_t vertexIndex;
        size_t triangleIndex;
        
        Vertex* vertices;
        GLshort* triangles;
        
        int drawCalls;
        int verticesRendered;
#ifdef DISABLE_MAP_BUFFER
		Vertex verts[MAX_VERTICES];
		GLshort indicies[MAX_TRIANGLES];
#endif
        void Render();
        
    private:
        
        GLuint batchVertexBuffer[3];
        GLuint batchIndexBuffer[3];
        
        void MapBuffers();
        
        int bufferIndex;
        
    };
}