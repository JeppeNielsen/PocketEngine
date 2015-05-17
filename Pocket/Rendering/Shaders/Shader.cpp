#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "VisibleObject.hpp"

using namespace Pocket;

Shader::Shader() {

}

Shader::~Shader() {

}

void Shader::Initialize() {

}

void Shader::Start(float *viewProjection) {
	shader.Use();
	OnStart(viewProjection);
}

void Shader::OnStart(float* viewProjection) { }


void Shader::Apply(VisibleObject* visibleObject) {

}

void Shader::Bind() {}

void Shader::BindTexture(Pocket::Texture *texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture ? texture->GetHandle() : NULL);
}

void Shader::InitBatching(const VisibleObject *visibleObject) {
	shaderBatching.Use();
    OnInitBatch(visibleObject);
}

void Shader::OnInitBatch(const VisibleObject* visibleObject) { }

bool Shader::RenderVertices(const VisibleObject* visibleObject, const Mesh* mesh, VertexRenderer& renderer) {
    
    size_t verticesSize = mesh->VerticesSize();
    size_t trianglesSize = mesh->TrianglesSize();
    
    renderer.Begin(verticesSize, trianglesSize);
    
    const Matrix4x4* world = visibleObject->transform->World.GetValue();
    size_t index = renderer.vertexIndex;
    for (size_t v=0; v<verticesSize; ++v) {
        const Vertex& source = mesh->vertices[v];
        Vertex& dest = renderer.vertices[renderer.vertexIndex++];
        world->TransformPositionAffine(source.Position, dest.Position);
        world->TransformVectorAffine(source.Normal, dest.Normal);
        //dest.Normal = source.Normal;
        dest.Color = source.Color;
        dest.TextureCoords.x = source.TextureCoords.x;
        dest.TextureCoords.y = source.TextureCoords.y;
    }
    for (size_t v = 0; v<trianglesSize; v++) {
        renderer.triangles[renderer.triangleIndex++] = (index + mesh->triangles[v]);
    }
    return false;
}

