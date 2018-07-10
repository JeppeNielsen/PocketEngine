//
//  VertexRenderer.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "OpenGL.hpp"

namespace Pocket {

class IVertexRenderer {
public:
    virtual ~IVertexRenderer() {}
};

template<class Vertex>
class VertexRenderer : public IVertexRenderer {
public:
    const static unsigned int MAX_VERTICES = 1<<16;
    const static unsigned int MAX_TRIANGLES = 1<<16;

    VertexRenderer() {
        ASSERT_GL(glGenBuffers(1, &vertexBuffer));
        ASSERT_GL(glGenBuffers(1, &indexBuffer));
        
        ASSERT_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
        ASSERT_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_VERTICES, 0, GL_DYNAMIC_DRAW));
        
        ASSERT_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
        ASSERT_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * MAX_TRIANGLES, 0, GL_DYNAMIC_DRAW));
    }

    ~VertexRenderer() {
        ASSERT_GL(glDeleteBuffers(1, &vertexBuffer));
        ASSERT_GL(glDeleteBuffers(1, &indexBuffer));
    }

public:
    
    void Begin(const size_t &vertexCount, const size_t &trianglesCount) {
        if (
        ((vertexCount + vertexIndex)>=MAX_VERTICES) ||
        ((trianglesCount + triangleIndex)>=MAX_TRIANGLES)
        )
        {
            Render();
        }
    }
    
    size_t vertexIndex;
    size_t triangleIndex;
    
    Vertex vertices[MAX_VERTICES];
    GLshort triangles[MAX_TRIANGLES];

    void Render() {
        if (vertexIndex==0) return;
        
        ASSERT_GL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
        ASSERT_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexIndex, vertices, GL_DYNAMIC_DRAW));
        
        ASSERT_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
        ASSERT_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * triangleIndex, triangles, GL_DYNAMIC_DRAW));
        
		ASSERT_GL(glDrawElements(GL_TRIANGLES, (int)triangleIndex, GL_UNSIGNED_SHORT, 0));

        drawCalls++;
        verticesRendered += vertexIndex;
        vertexIndex = 0;
        triangleIndex = 0;
    }
    
    void BeginLoop() {
        vertexIndex = 0;
        triangleIndex = 0;
        drawCalls=0;
        verticesRendered = 0;
    }
    
    void EndLoop() {
        Render();
    }

    int drawCalls;
    int verticesRendered;

private:
    GLuint vertexBuffer;
    GLuint indexBuffer;
};

}
