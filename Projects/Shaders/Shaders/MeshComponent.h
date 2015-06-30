//
//  MeshComponent.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "VertexMesh.h"
#include "BoundingBox.hpp"
#include "DirtyProperty.hpp"

using namespace Pocket;

class RenderSystem;

Component(MeshComponent)
public:
    
    MeshComponent() : vertexMesh(0), vertexType(0), LocalBoundingBox(this) {}
    ~MeshComponent() { delete vertexMesh; }

    void Reset() {
        delete vertexMesh;
        vertexMesh = 0;
    }
    
    template<class Vertex>
    class VertexMesh<Vertex>& Mesh() {
        if (!vertexMesh) {
            vertexMesh = new VertexMesh<Vertex>();
            vertexType = Vertex::ID;
            RevertDefaultCalcBoundingBox();
        } else if (vertexType != Vertex::ID) {
            return VertexMesh<Vertex>::empty;
        }
        VertexMesh<Vertex>& mesh = *((VertexMesh<Vertex>*)vertexMesh);
        LocalBoundingBox.MakeDirty();
        return mesh;
    }

    template<class Vertex>
    const VertexMesh<Vertex>& ConstMesh() const {
        return (const VertexMesh<Vertex>&)*((VertexMesh<Vertex>*)vertexMesh);
    }
    
    size_t VerticesSize() {
        return vertexMesh ? vertexMesh->Size() : 0;
    }
    
    const Vector3& Position(size_t index) {
        return vertexMesh ? vertexMesh->GetPosition(index) : Vector3::zero;
    }
    
    int VertexType() { return vertexType; }
    
    IVertexMesh::Triangles& Triangles() { return vertexMesh->triangles; }
    DirtyProperty<MeshComponent*, BoundingBox> LocalBoundingBox;

private:
    IVertexMesh* vertexMesh;
    int vertexType;


    void RevertDefaultCalcBoundingBox() {
        LocalBoundingBox.Method.Clear();
        LocalBoundingBox.Method += event_handler(this, &MeshComponent::CalcBoundingBox);
    }

    void CalcBoundingBox( DirtyProperty<MeshComponent*, BoundingBox>::EventData& eventData )
    {
        BoundingBox& box = *eventData.Value;
        vertexMesh->CalcBoundingBox(box);
    }

    friend class RenderSystem;
};