//
//  Vertex.cpp
//  Shaders
//
//  Created by Jeppe Nielsen on 03/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Vertex.hpp"
#include "ObjectRenderer.hpp"

using namespace Pocket;

int IVertexType::counter = 0;
IVertexType::TypeList IVertexType::typelist;

template<> const VertexAttribute VertexAttributeType<Vector3>::attribute {  3, GL_FLOAT, GL_FALSE };
template<> const VertexAttribute VertexAttributeType<Vector2>::attribute {  2, GL_FLOAT, GL_FALSE };
template<> const VertexAttribute VertexAttributeType<Colour>::attribute {  4, GL_UNSIGNED_BYTE, GL_TRUE };

template<> void ShaderVariableType<float>::SetValue(GLint location, const float &value) {
    glUniform1f(location, value);
}
template<> GLenum ShaderVariableType<float>::GetType() { return GL_FLOAT; }

template<> void ShaderVariableType<Vector2>::SetValue(GLint location, const Vector2 &value) {
    glUniform2f(location, value.x, value.y);
}
template<> GLenum ShaderVariableType<Vector2>::GetType() { return GL_FLOAT_VEC2; }

template<> void ShaderVariableType<Vector3>::SetValue(GLint location, const Vector3 &value) {
    glUniform3f(location, value.x, value.y, value.z);
}
template<> GLenum ShaderVariableType<Vector3>::GetType() { return GL_FLOAT_VEC3; }

template<> void ShaderVariableType<Matrix4x4>::SetValue(GLint location, const Matrix4x4 &value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value.GetGlMatrix());
}
template<> GLenum ShaderVariableType<Matrix4x4>::GetType() { return GL_FLOAT_MAT4; }

template<> void ShaderVariableType<Colour>::SetValue(GLint location, const Colour &value) {
    glUniform4f(location, value.R(), value.G(), value.B(), value.A());
}
template<> GLenum ShaderVariableType<Colour>::GetType() { return GL_FLOAT_VEC4; }

int Vertex::ID = VertexType<Vertex>::Id();
VertexDescription<Vertex> Vertex::Description;

template<> IObjectRenderer* VertexType<Vertex>::CreateRenderer() {
    return new ObjectRenderer<Vertex>();
}