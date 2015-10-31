//
//  SpringMeshSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SpringMeshSystem.h"
#include "MathHelper.hpp"

void SpringMeshSystem::Initialize() {
    AddComponent<Spring>();
    AddComponent<Mesh>();
}

void SpringMeshSystem::ObjectAdded(GameObject* object) {
    
    auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
    
    mesh.vertices.clear();
    mesh.triangles.clear();
    
    mesh.triangles.push_back(0);
    mesh.triangles.push_back(2);
    mesh.triangles.push_back(1);
    mesh.triangles.push_back(0);
    mesh.triangles.push_back(3);
    mesh.triangles.push_back(2);
    
    mesh.vertices.resize(4, Vertex());
    mesh.vertices[0].Position = {-0.1f, 0,0};
    mesh.vertices[1].Position = {0.1f, 0,0};
    mesh.vertices[2].Position = {0.1f, 0,0};
    mesh.vertices[3].Position = {-0.1f, 0,0};
    
}

void SpringMeshSystem::Update(float dt) {
    for (auto it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
        
        Spring* spring = object->GetComponent<Spring>();
        
        float lengthHalf = spring->currentLength * 0.5f;
        
        auto& verts = mesh.vertices;
        
        verts[0].Position.y = lengthHalf;
        verts[1].Position.y = lengthHalf;
        verts[2].Position.y = -lengthHalf;
        verts[3].Position.y = -lengthHalf;
        
        float normalizedTension = MathHelper::Clamp(spring->tension*0.5f, -1.0f, 1.0f);
        Colour color;
        if (normalizedTension<0) {
            color = Colour::Lerp(Colour(1.0f,1.0f,1.0f,1.0f), Colour(0.0f,0.0f,1.0f,1.0f), -normalizedTension);
        } else {
            color = Colour::Lerp(Colour(1.0f,1.0f,1.0f,1.0f), Colour(1.0f,0.0f,0.0f,1.0f), normalizedTension);
        }
        
        verts[0].Color = color;
        verts[1].Color = color;
        verts[2].Color = color;
        verts[3].Color = color;
        
    }
}
