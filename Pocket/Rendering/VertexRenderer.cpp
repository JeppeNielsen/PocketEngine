//
//  RenderCall.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/22/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "VertexRenderer.hpp"
#include "Shader.hpp"



using namespace Pocket;

#define MAX_BUFFERS 1

VertexRenderer::VertexRenderer() {
    for (int i=0; i<MAX_BUFFERS; i++) {
        glGenBuffers(1, &batchVertexBuffer[i]);
        glGenBuffers(1, &batchIndexBuffer[i]);
        
        glBindBuffer(GL_ARRAY_BUFFER, batchVertexBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_VERTICES, 0, GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchIndexBuffer[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * MAX_TRIANGLES, 0, GL_DYNAMIC_DRAW);
    }
    
    bufferIndex = 0;
}

VertexRenderer::~VertexRenderer() {
    
}

void VertexRenderer::Init() {
    
    drawCalls = 0;
    vertexIndex = 0;
    triangleIndex = 0;
    shader = 0;
    visibleObject = 0;
    verticesRendered = 0;
    
    MapBuffers();
}

void VertexRenderer::MapBuffers() {
    
    //bufferIndex = 0;
    //bufferIndex++;
    //if (bufferIndex==0) bufferIndex = 0;
    
    glBindBuffer(GL_ARRAY_BUFFER, batchVertexBuffer[bufferIndex]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchIndexBuffer[bufferIndex]);
    


#ifdef DISABLE_MAP_BUFFER
	vertices = &verts[0];
	triangles = &indicies[0];
#else
    vertices = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    triangles = (GLshort*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
#endif
    
}

void VertexRenderer::Begin(const size_t &vertexCount, const size_t &trianglesCount) {
    
    if (
    ((vertexCount + vertexIndex)>=MAX_VERTICES) ||
    ((trianglesCount + triangleIndex)>=MAX_TRIANGLES)
    )
    {
        Render();
    }
}

void VertexRenderer::Render() {
    if (vertexIndex==0) return;

    //glBindBuffer(GL_ARRAY_BUFFER, batchVertexBuffer[bufferIndex]);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchIndexBuffer[bufferIndex]);
    
#ifdef DISABLE_MAP_BUFFER
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertexIndex, vertices);
	//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort) * triangleIndex, triangles);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexIndex, vertices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * triangleIndex, triangles, GL_DYNAMIC_DRAW);
#else
	glUnmapBuffer(GL_ARRAY_BUFFER);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
#endif
    
    if (shader) {
        shader->InitBatching(visibleObject);
    }
    
    glDrawElements(GL_TRIANGLES, (int)triangleIndex, GL_UNSIGNED_SHORT, 0);
    drawCalls++;
    verticesRendered += vertexIndex;
    
    vertexIndex = 0;
    triangleIndex = 0;

    MapBuffers();
}

