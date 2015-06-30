//
//  MeshComponent.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Vertex.h"

using namespace Pocket;

class MeshComponent : public GameComponent<MeshComponent> {
public:
    
    MeshComponent() : vertices(0), vertexType(0) {}
    ~MeshComponent() { delete vertices; }
    
    virtual std::string Name() { return "Mesh"; }
    static std::string NameStatic() { return "Mesh"; }

    typedef std::vector<short> TrianglesList;
    
    void Reset() {
        delete vertices;
        triangles.clear();
        vertices = 0;
    }
    
    template<class Vertex>
    typename VertexCollection<Vertex>::Vertices& Vertices() {
        if (!vertices) {
            vertices = new VertexCollection<Vertex>();
            vertexType = Vertex::ID;
        } else if (vertexType != Vertex::ID) {
            VertexCollection<Vertex>::empty.clear();
            return VertexCollection<Vertex>::empty;
        }
        VertexCollection<Vertex>& collection = *((VertexCollection<Vertex>*)vertices);
        return collection.vertices;
    }
    
    size_t Count() {
        return vertices ? vertices->Count() : 0;
    }
    
    const Vector3& Position(size_t index) {
        return vertices ? vertices->GetPosition(index) : Vector3::zero;
    }
    
    int VertexType() { return vertexType; }
    TrianglesList& Triangles() { return triangles; }
    
private:
    TrianglesList triangles;
    IVertexCollection* vertices;
    int vertexType;
};