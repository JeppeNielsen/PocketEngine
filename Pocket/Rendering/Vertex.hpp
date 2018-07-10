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
#include "OpenGL.hpp"
#include <vector>
#include <map>
#include "StringHelper.hpp"

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
	VertexAttribute() {}
	VertexAttribute(GLint size, GLenum type, GLboolean normalized) :
		size(size), type(type), normalized(normalized) {}
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

template<typename T> const VertexAttribute VertexAttributeType<T>::attribute;

template<class Vertex>
class VertexDescription {
public:
    VertexDescription() {
        id = Vertex::ID;
        stride = sizeof(Vertex);
        startOffset = (size_t)(&vertex);
        vertex.Initialize(*this);
    
        //for(auto& a : attributes) {
        //    std::cout<<"Name : " << a.first<< "  offset : " <<a.second.offset<<std::endl;
        //}
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
    
    friend std::ostream& operator<<(std::ostream& stream, const Vertex& v) {
        stream << v.Position;
        stream << ":";
        stream << v.TextureCoords;
        stream << ":";
        stream << v.Color;
        stream << ":";
        stream << v.Normal;
        return stream;
    }
    
    Vertex static Deserialize(const std::string& data, const char delim = ',') {
    
        auto splitted = StringHelper::split(data, ':');
        
        Vertex v;
        if (splitted.size()>0) {
            v.Position = Vector3::Deserialize(splitted[0]);
        }
        if (splitted.size()>1) {
            v.TextureCoords = Vector2::Deserialize(splitted[1]);
        }
        if (splitted.size()>2) {
            v.Color = Colour::Deserialize(splitted[2]);
        }
        if (splitted.size()>3) {
            v.Normal = Vector3::Deserialize(splitted[3]);
        }
        return v;
    }
    
    inline bool operator ==(const Vertex& other) const{
        return !(Position!=other.Position ||
                 TextureCoords!=other.TextureCoords ||
                 Color!=other.Color ||
                 Normal!=other.Normal);
    }
    
    inline bool operator !=(const Vertex& other) const{
        return (Position!=other.Position ||
                 TextureCoords!=other.TextureCoords ||
                 Color!=other.Color ||
                 Normal!=other.Normal);
    }

};

}