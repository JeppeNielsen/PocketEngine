//
//  PatchMeshSystem.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/17/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PatchMeshSystem.hpp"
#include "MathHelper.hpp"

void PatchMeshSystem::Initialize() {
    AddComponent<Patch>();
    AddComponent<Mesh>();
}

void PatchMeshSystem::ObjectAdded(Pocket::GameObject *object) {
    CreateMesh(object->GetComponent<Patch>(), object->GetComponent<Mesh>());
}

void PatchMeshSystem::ObjectRemoved(Pocket::GameObject *object) {
    
}

void PatchMeshSystem::CreateMesh(Patch* patch, Pocket::Mesh *mesh) {
    
    auto& vertices = mesh->GetMesh<Vertex>().vertices;
    auto& triangles = mesh->GetMesh<Vertex>().triangles;
    
    vertices.clear();
    triangles.clear();
    
    Colour color = Colour((Colour::Component)84,84,84);
    
    vertices.push_back({Vector3(1.5f, 1.5f, -0.5f), Vector2(0,0), color, Vector3(0,0,1)});
    vertices.push_back({Vector3(-1.5f, 1.5f, -0.5f), Vector2(0,0), color, Vector3(0,0,1)});
    vertices.push_back({Vector3(-1.5f, -1.5f, -0.5f), Vector2(0,0), color, Vector3(0,0,1)});
    vertices.push_back({Vector3(1.5f, -1.5f, -0.5f), Vector2(0,0), color, Vector3(0,0,1)});
        
    triangles.push_back((short)0);
    triangles.push_back((short)1);
    triangles.push_back((short)2);

    triangles.push_back((short)0);
    triangles.push_back((short)2);
    triangles.push_back((short)3);
    
    
    
        
    
}