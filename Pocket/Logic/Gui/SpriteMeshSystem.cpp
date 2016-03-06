//
//  SpriteMeshSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#include "SpriteMeshSystem.hpp"

using namespace Pocket;

void SpriteMeshSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Sizeable>()->Size.Changed.Bind(this, &SpriteMeshSystem::UpdateMesh, object);
    object->GetComponent<Sprite>()->CornerSize.Changed.Bind(this, &SpriteMeshSystem::UpdateMesh, object);
    UpdateMesh(object);
}

void SpriteMeshSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Sizeable>()->Size.Changed.Unbind(this, &SpriteMeshSystem::UpdateMesh, object);
    object->GetComponent<Sprite>()->CornerSize.Changed.Unbind(this, &SpriteMeshSystem::UpdateMesh, object);
}

void SpriteMeshSystem::UpdateMesh(GameObject* object) {
    const Vector2& size = object->GetComponent<Sizeable>()->Size;
    Mesh* mesh = object->GetComponent<Mesh>();
    Sprite* sprite = object->GetComponent<Sprite>();
    const Vector2& wantedCornerSize = sprite->CornerSize;
    
    auto& vertices = mesh->GetMesh<Vertex>().vertices;
    auto& triangles = mesh->GetMesh<Vertex>().triangles;
    
    bool normal = wantedCornerSize.x<=0 && wantedCornerSize.y<=0;
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
    } else {
        
        Vector2 cornerSize;
        
        if (size.x<wantedCornerSize.x*2) {
            cornerSize.x = size.x * 0.5f;
        } else {
            cornerSize.x = wantedCornerSize.x;
        }
        
        if (size.y<wantedCornerSize.y*2) {
            cornerSize.y = size.y * 0.5f;
        } else {
            cornerSize.y = wantedCornerSize.y;
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
    }
}
