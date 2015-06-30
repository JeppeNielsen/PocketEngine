//
//  Vertex.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <stdio.h>
#include "Vector3.hpp"
#include "Colour.hpp"
#include "Matrix4x4.hpp"
#include "OpenGl.hpp"
#include <vector>
#include <map>

using namespace Pocket;

class IObjectRenderer;

class IVertexCollection {
public:
    virtual ~IVertexCollection() { }
    virtual const Vector3& GetPosition(size_t index) = 0;
    virtual size_t Count() = 0;
};

template<class Vertex>
class VertexCollection : public IVertexCollection {
public:
    typedef std::vector<Vertex> Vertices;
    Vertices vertices;
    const Vector3& GetPosition(size_t index) {
        return vertices[index].Position;
    }
    size_t Count() { return vertices.size(); }
    static Vertices empty;
};

template<class Vertex> typename VertexCollection<Vertex>::Vertices VertexCollection<Vertex>::empty;

struct IVertexType {
    typedef std::vector<IVertexType*> TypeList;
    static TypeList typelist;
    static int counter;
    virtual IVertexCollection* CreateCollection() = 0;
    virtual IObjectRenderer* CreateRenderer() = 0;
};

int IVertexType::counter = 0;
IVertexType::TypeList IVertexType::typelist;

template<class Vertex>
struct VertexType : IVertexType {
    static int Id() {
        int id = IVertexType::counter++;
        IVertexType::typelist.resize(id + 1);
        IVertexType::typelist[id] = new VertexType<Vertex>();
        return id;
    }
    
    virtual IVertexCollection* CreateCollection() { return 0; }
    virtual IObjectRenderer* CreateRenderer();
};

struct VertexAttribute {
    GLint size;
    GLenum type;
    GLboolean normalized;
    int offset;
    GLint glAttribute;
};

template<class T>
struct VertexAttributeType {
    static const VertexAttribute attribute;
};

template<> const VertexAttribute VertexAttributeType<Vector3>::attribute {  3, GL_FLOAT, GL_FALSE };
template<> const VertexAttribute VertexAttributeType<Vector2>::attribute {  2, GL_FLOAT, GL_FALSE };
template<> const VertexAttribute VertexAttributeType<Colour>::attribute {  4, GL_UNSIGNED_BYTE, GL_TRUE };

template<class Vertex>
class VertexDescription {
public:
    VertexDescription() {
        id = Vertex::ID;
        stride = sizeof(Vertex);
        startOffset = (size_t)(&vertex);
        vertex.Initialize(*this);
    
        for(auto& a : attributes) {
            std::cout<<"Name : " << a.first<< "  offset : " <<a.second.offset<<std::endl;
        }
    }
    
public:
    const int Stride() { return stride; }
private:
    int stride;
    Vertex vertex;
    size_t startOffset;
    typedef std::map<std::string, VertexAttribute> AttributesList;
    AttributesList attributes;
    int id;
public:

    const AttributesList& Attributes() { return attributes; }

    template<class T>
    void SetAttibute(T& member, std::string name) {
        VertexAttribute attribute = VertexAttributeType<T>::attribute;
        attribute.offset = (int)(((size_t)&member) - startOffset);
        attributes[name] = attribute;
    }
};

template<class T>
struct ShaderVariableType {
    static void SetValue(GLint location, const T& value);
    static GLenum GetType();
};


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



struct Vertex {
    Vector3 Position;
    Vector3 Normal;
    Vector2 TextureCoords;
    Colour Color;

    void Initialize(VertexDescription<Vertex>& description) {
        description.SetAttibute(Position, "Position");
        description.SetAttibute(Normal, "Normal");
        description.SetAttibute(TextureCoords, "TexCoords");
        description.SetAttibute(Color, "Color");
    }
    
    static int ID;
    static VertexDescription<Vertex> Description;
};

int Vertex::ID = VertexType<Vertex>::Id();
VertexDescription<Vertex> Vertex::Description;




