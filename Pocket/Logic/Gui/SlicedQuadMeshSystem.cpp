//
//  SlicedQuadMeshSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#include "SlicedQuadMeshSystem.hpp"

using namespace Pocket;

void SlicedQuadMeshSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Sizeable>()->Size.Changed.Bind(this, &SlicedQuadMeshSystem::UpdateMesh, object);
    object->GetComponent<SlicedQuad>()->InnerSize.Changed.Bind(this, &SlicedQuadMeshSystem::UpdateMesh, object);
    object->GetComponent<SlicedQuad>()->OuterSize.Changed.Bind(this, &SlicedQuadMeshSystem::UpdateMesh, object);
    UpdateMesh(object);
}

void SlicedQuadMeshSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Sizeable>()->Size.Changed.Unbind(this, &SlicedQuadMeshSystem::UpdateMesh, object);
    object->GetComponent<SlicedQuad>()->InnerSize.Changed.Unbind(this, &SlicedQuadMeshSystem::UpdateMesh, object);
    object->GetComponent<SlicedQuad>()->OuterSize.Changed.Unbind(this, &SlicedQuadMeshSystem::UpdateMesh, object);
}

void SlicedQuadMeshSystem::UpdateMesh(GameObject* object) {
    const Vector2& size = object->GetComponent<Sizeable>()->Size;
    Mesh* mesh = object->GetComponent<Mesh>();
    SlicedQuad* slicedQuad = object->GetComponent<SlicedQuad>();
    //float outer = sprite->OuterSize;
    float inner = slicedQuad->InnerSize;
    
    auto& vertices = mesh->GetMesh<Vertex>().vertices;
    auto& triangles = mesh->GetMesh<Vertex>().triangles;
    
    bool normal = inner>-0.001f && inner<0.001f; ;//outer == inner;
    int vertCount = normal ? 4 : 16;
    int trianglesCount = normal ? 6 : 54;
    
    if (vertices.size()!=vertCount) {
        vertices.resize(vertCount);
    }
    
    if (triangles.size()!=trianglesCount) {
        triangles.resize(trianglesCount, 0);
        
        if (normal) {
            triangles[0] = 0;
            triangles[1] = 1;
            triangles[2] = 2;
            triangles[3] = 0;
            triangles[4] = 2;
            triangles[5] = 3;
        } else {
            
            int index = 0;
            for (int y=0; y<3; y++) {
                for (int x=0; x<3; x++) {
                    int start = x + y * 4;
                    triangles[index++] = (GLshort)(start);
                    triangles[index++] = (GLshort)(start+1);
                    triangles[index++] = (GLshort)(start+5);
                    triangles[index++] = (GLshort)(start);
                    triangles[index++] = (GLshort)(start+5);
                    triangles[index++] = (GLshort)(start+4);
                }
            }
        }
    }
    
    if (normal) {
    
        vertices[0].Position = Vector3(0,0,0);
        vertices[1].Position = Vector3(size.x,0,0);
        vertices[2].Position = Vector3(size.x,size.y,0);
        vertices[3].Position = Vector3(0,size.y,0);
        
        vertices[0].TextureCoords = Vector2(0,0);
        vertices[1].TextureCoords = Vector2(1,0);
        vertices[2].TextureCoords = Vector2(1,1);
        vertices[3].TextureCoords = Vector2(0,1);
        
    } else {
    
        Vector2 cornerSize = inner;
        
        if (size.x<inner*2) {
            cornerSize.x = size.x * 0.5f;
        }
        
        if (size.y<inner*2) {
            cornerSize.y = size.y * 0.5f;
        }
        
        
        vertices[0].Position = Vector3(0,0,0);
        vertices[1].Position = Vector3(cornerSize.x,0,0);
        vertices[2].Position = Vector3(size.x - cornerSize.x,0,0);
        vertices[3].Position = Vector3(size.x,0,0);
        
        vertices[4].Position = Vector3(0,cornerSize.y,0);
        vertices[5].Position = Vector3(cornerSize.x,cornerSize.y,0);
        vertices[6].Position = Vector3(size.x - cornerSize.x,cornerSize.y,0);
        vertices[7].Position = Vector3(size.x,cornerSize.y,0);
        
        vertices[ 8].Position = Vector3(0,size.y - cornerSize.y,0);
        vertices[ 9].Position = Vector3(cornerSize.x,size.y - cornerSize.y,0);
        vertices[10].Position = Vector3(size.x - cornerSize.x,size.y - cornerSize.y,0);
        vertices[11].Position = Vector3(size.x,size.y - cornerSize.y,0);
        
        vertices[12].Position = Vector3(0,size.y,0);
        vertices[13].Position = Vector3(cornerSize.x,size.y,0);
        vertices[14].Position = Vector3(size.x - cornerSize.x,size.y,0);
        vertices[15].Position = Vector3(size.x,size.y,0);
        
        SlicedTexture* slicedTexture = object->GetComponent<SlicedTexture>();
        
        const Box& base = slicedTexture->BaseArea;
        const Box& inner = slicedTexture->InnerArea;
        
        vertices[0].TextureCoords = Vector2(base.left, base.bottom);
        vertices[1].TextureCoords = Vector2(inner.left, base.bottom);
        vertices[2].TextureCoords = Vector2(inner.right, base.bottom);
        vertices[3].TextureCoords = Vector2(base.right, base.bottom);
        
        vertices[4].TextureCoords = Vector2(base.left, inner.bottom);
        vertices[5].TextureCoords = Vector2(inner.left, inner.bottom);
        vertices[6].TextureCoords = Vector2(inner.right, inner.bottom);
        vertices[7].TextureCoords = Vector2(base.right, inner.bottom);
        
        vertices[ 8].TextureCoords = Vector2(base.left, inner.top);
        vertices[ 9].TextureCoords = Vector2(inner.left, inner.top);
        vertices[10].TextureCoords = Vector2(inner.right, inner.top);
        vertices[11].TextureCoords = Vector2(base.right, inner.top);
        
        vertices[12].TextureCoords = Vector2(base.left, base.top);
        vertices[13].TextureCoords = Vector2(inner.left, base.top);
        vertices[14].TextureCoords = Vector2(inner.right, base.top);
        vertices[15].TextureCoords = Vector2(base.right, base.top);
        
    }
}
