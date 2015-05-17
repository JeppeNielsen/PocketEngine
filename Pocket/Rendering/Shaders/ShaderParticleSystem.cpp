//
//  ShaderParticleSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/14/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "ShaderParticleSystem.hpp"
#include "VisibleObject.hpp"
#include "ParticleEmitter.hpp"
using namespace Pocket;
using namespace std;

void ShaderParticleSystem::Initialize() {
    
    vertexStride = sizeof(Vertex);
    lastTexture = 0;
    
	string vertexShader =
#ifdef OPENGL_DESKTOP
    "#version 120 \n"
#endif
    "attribute vec4 Position;\n"
    "attribute vec2 TexCoords;\n"
    "attribute vec4 Color;\n"
    "attribute vec3 Size;\n"
    "uniform mat4 ViewProjection;\n"
    "uniform mat4 World;\n"
    "varying vec2 DestinationTexCoords;\n"
    "varying vec4 DestinationColor;\n"
    "void main(void) {\n"
    "	DestinationTexCoords = TexCoords;\n"
    "	DestinationColor = Color;\n"
    "	gl_Position = Position * World * ViewProjection;\n"
    "}\n";
    
	string vertexShaderBatching =
#ifdef OPENGL_DESKTOP
    "#version 120 \n"
#endif
    "attribute vec4 Position;\n"
    "attribute vec2 TexCoords;\n"
    "attribute vec4 Color;\n"
    "attribute vec3 Size;\n"
    "uniform mat4 ViewProjection;\n"
    "uniform float Aspect; \n"
    "varying vec2 DestinationTexCoords;\n"
    "varying vec4 DestinationColor;\n"
    "void main(void) {\n"
    "	DestinationTexCoords = TexCoords;\n"
    "	DestinationColor = Color;\n"
    "   vec4 pos = Position * ViewProjection;  \n"
    "   pos.x += Size.x; \n"
    "   pos.y += Size.y * Aspect; \n"
    "	gl_Position = pos;\n"
    "}\n";
    
	string fragmentShader =
#ifdef OPENGL_DESKTOP
    "#version 120 \n"
    "varying vec2 DestinationTexCoords;"
    "varying vec4 DestinationColor;"
#else
    "varying lowp vec2 DestinationTexCoords;"
    "varying lowp vec4 DestinationColor;"
#endif
    "uniform sampler2D Texture;"
    "void main(void) {"
    "	gl_FragColor = texture2D(Texture, DestinationTexCoords) * DestinationColor;"
    "}";
    
	if (!shader.CreateShaders(vertexShader, fragmentShader)) return;
    if (!shaderBatching.CreateShaders(vertexShaderBatching, fragmentShader)) return;
    
	positionSlot = shader.CreateAttribVariable("Position");
    sizeSlot = shader.CreateAttribVariable("Size");
	textureCoordSlot = shader.CreateAttribVariable("TexCoords");
	colorSlot = shader.CreateAttribVariable("Color");
    
	viewProjectionSlot = shader.CreateUniformVariable("ViewProjection");
	worldSlot = shader.CreateUniformVariable("World");
	textureSlot = shader.CreateUniformVariable("Texture");
    
    
    positionSlot_batched = shaderBatching.CreateAttribVariable("Position");
    sizeSlot_batched = shaderBatching.CreateAttribVariable("Size");
	textureCoordSlot_batched = shaderBatching.CreateAttribVariable("TexCoords");
	colorSlot_batched = shaderBatching.CreateAttribVariable("Color");
    
	viewProjectionSlot_batched = shaderBatching.CreateUniformVariable("ViewProjection");
	textureSlot_batched = shaderBatching.CreateUniformVariable("Texture");
    aspectSlot_batched = shaderBatching.CreateUniformVariable("Aspect");
}

void ShaderParticleSystem::OnStart(float* viewProjection) {
    glUniformMatrix4fv(viewProjectionSlot, 1, GL_FALSE, viewProjection);
    glUniform1i(textureSlot, 0);
    lastTexture = 0;
}

void ShaderParticleSystem::Bind() {
    glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	glVertexAttribPointer(textureCoordSlot, 2, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)12);
	glVertexAttribPointer(colorSlot, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
    glVertexAttribPointer(sizeSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)24);
}

void ShaderParticleSystem::Apply(VisibleObject* visibleObject) {
    
    glUniformMatrix4fv(worldSlot, 1, GL_FALSE, visibleObject->transform->World.GetValue()->GetGlMatrix());
    
    if (visibleObject->texture!=lastTexture) {
        lastTexture = visibleObject->texture;
        glBindTexture(GL_TEXTURE_2D, lastTexture->Texture().GetHandle());
    }
}


void ShaderParticleSystem::OnInitBatch(const Pocket::VisibleObject *visibleObject) {
    
    glUniformMatrix4fv(viewProjectionSlot_batched, 1, GL_FALSE, viewProjection);
    glUniform1i(textureSlot_batched, 0);
    glUniform1f(aspectSlot_batched, Aspect);
    
    glVertexAttribPointer(positionSlot_batched, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	glVertexAttribPointer(textureCoordSlot_batched, 2, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)12);
	glVertexAttribPointer(colorSlot_batched, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
    glVertexAttribPointer(sizeSlot_batched, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)24);
    
}

bool ShaderParticleSystem::RenderVertices(const VisibleObject* visibleObject, const Mesh* mesh, VertexRenderer& renderer) {
        
    size_t verticesSize = mesh->VerticesSize();
    size_t trianglesSize = mesh->TrianglesSize();
    
    renderer.Begin(verticesSize, trianglesSize);
    
    size_t index = renderer.vertexIndex;
    
    const Matrix4x4* world = visibleObject->transform->World.GetValue();
    float scale = world->TransformVector(Vector3(0,0,1)).Length();
    
    ParticleEmitter* emitter = visibleObject->object->GetComponent<ParticleEmitter>();
    bool useWorldSpace = emitter && emitter->UseWorldSpace();
    
    if (useWorldSpace) {
        for (size_t v=0; v<verticesSize; ++v) {
            const Vertex& source = mesh->vertices[v];
            Vertex& dest = renderer.vertices[renderer.vertexIndex++];
            dest.Position = source.Position;
            dest.Normal = source.Normal * scale;
            dest.Color = source.Color;
            dest.TextureCoords.x = source.TextureCoords.x;
            dest.TextureCoords.y = source.TextureCoords.y;
        }
    } else {
        for (size_t v=0; v<verticesSize; ++v) {
            const Vertex& source = mesh->vertices[v];
            Vertex& dest = renderer.vertices[renderer.vertexIndex++];
            world->TransformPositionAffine(source.Position, dest.Position);
            dest.Normal = source.Normal * scale;
            dest.Color = source.Color;
            dest.TextureCoords.x = source.TextureCoords.x;
            dest.TextureCoords.y = source.TextureCoords.y;
        }
    }
    for (size_t v = 0; v<trianglesSize; v++) {
        renderer.triangles[renderer.triangleIndex++] = (index + mesh->triangles[v]);
    }

    return false;
}





