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

namespace Pocket {

class IObjectRenderer;

struct IVertexType {
    typedef std::vector<IVertexType*> TypeList;
    static TypeList typelist;
    static int counter;
    virtual IObjectRenderer* CreateRenderer() = 0;
};

template<class Vertex>
struct VertexType : IVertexType {
    static int Id() {
        int id = IVertexType::counter++;
        IVertexType::typelist.resize(id + 1);
        IVertexType::typelist[id] = new VertexType<Vertex>();
        return id;
    }
    
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




struct Vertex {
    Vector3 Position;
    Vector2 TextureCoords;
    Colour Color;
    Vector3 Normal;

    void Initialize(VertexDescription<Vertex>& description) {
        description.SetAttibute(Position, "Position");
        description.SetAttibute(TextureCoords, "TexCoords");
        description.SetAttibute(Color, "Color");
        description.SetAttibute(Normal, "Normal");
    }
    
    static int ID;
    static VertexDescription<Vertex> Description;
};

}