//
//  ParticleMeshSystem.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 31/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ParticleMeshSystem.h"

void ParticleMeshSystem::ObjectAdded(Pocket::GameObject *object) {
    auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
    Atlas* atlas = object->GetComponent<Atlas>();
    
    mesh.triangles.resize(6);
    mesh.triangles.push_back(0);
    mesh.triangles.push_back(2);
    mesh.triangles.push_back(1);
    
    mesh.triangles.push_back(0);
    mesh.triangles.push_back(3);
    mesh.triangles.push_back(2);
    
    const Atlas::Node& node = atlas->GetNode("Bolt");
    
    mesh.vertices.resize(4);
    
    mesh.vertices[0].TextureCoords = Vector2(node.outer.left,node.outer.bottom);
    mesh.vertices[1].TextureCoords = Vector2(node.outer.right,node.outer.bottom);
    mesh.vertices[2].TextureCoords = Vector2(node.outer.right,node.outer.top);
    mesh.vertices[3].TextureCoords = Vector2(node.outer.left,node.outer.top);
    
    const float size = 1.4f;
    
    mesh.vertices[0].Position = {-size,-size,0};
    mesh.vertices[1].Position = {-size,size,0};
    mesh.vertices[2].Position = {size,size,0};
    mesh.vertices[3].Position = {size,-size,0};
}

void ParticleMeshSystem::ObjectRemoved(Pocket::GameObject *object) {
    
}