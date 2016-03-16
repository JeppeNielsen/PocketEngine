//
//  ParticleMeshUpdater.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/14/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "ParticleMeshUpdater.hpp"
#include "Vertex.hpp"

using namespace Pocket;

void ParticleMeshUpdater::Update(float dt) {
    for (auto go : Objects()) {
        UpdateMesh(go->GetComponent<Mesh>(), go->GetComponent<ParticleEmitter>());
    }
}

void ParticleMeshUpdater::UpdateMesh(Mesh *mesh, ParticleEmitter *emitter) {
    if (!emitter->Playing()) return;
    
    int vertCount = (int)emitter->particles.size() * 4;
    int triCount = (int)emitter->particles.size() * 6;
    
    auto& vertexMesh = mesh->GetMesh<Vertex>();
    
    auto& vertices = vertexMesh.vertices;
    auto& triangles = vertexMesh.triangles;
        
    if (triangles.size()!=triCount) {
        triangles.resize(triCount);
        int index = 0;
        for (int i=0; i<triCount; i+=6) {
            triangles[i] = (GLshort)(index+0);
            triangles[i+1] = (GLshort)(index+1);
            triangles[i+2] = (GLshort)(index+2);
            
            triangles[i+3] = (GLshort)(index+0);
            triangles[i+4] = (GLshort)(index+2);
            triangles[i+5] = (GLshort)(index+3);
            
            index+=4;
        }
    }
    
    if (vertices.size()!=vertCount) {
        vertices.resize(vertCount);
    }
    
    //mesh->SetBufferSizes(particlesCount * 4, particlesCount * 6); // not sure about this

    int index = 0;
    Vector3 vector;
    for (size_t i=0; i<emitter->activeParticles.size(); ++i) {
        ParticleEmitter::Particle* particle = emitter->activeParticles[i];
        
        vertices[index  ].Position = particle->position;// + Vector3(-particle->size, -particle->size,0);
        vertices[index+1].Position = particle->position;// + Vector3(particle->size, -particle->size,0);
        vertices[index+2].Position = particle->position;// + Vector3(particle->size, particle->size,0);
        vertices[index+3].Position = particle->position;// + Vector3(-particle->size, particle->size,0);
        
        vertices[index  ].TextureCoords = Vector2(0,0);
        vertices[index+1].TextureCoords = Vector2(1,0);
        vertices[index+2].TextureCoords = Vector2(1,1);
        vertices[index+3].TextureCoords = Vector2(0,1);
        
        vertices[index  ].Color = particle->color;
        vertices[index+1].Color = particle->color;
        vertices[index+2].Color = particle->color;
        vertices[index+3].Color = particle->color;
        
        /*
                
        vector.x = cosf(particle->rotation+3.14159265359f/4) * particle->size/sqrtf(2.0f);
        vector.y = sinf(particle->rotation+3.14159265359f/4) * particle->size/sqrtf(2.0f);
                
        vertices[index  ].Normal = Vector3(vector.x, vector.y,0);
        vertices[index+3].Normal = Vector3(vector.y, -vector.x,0);
        vertices[index+2].Normal = Vector3(-vector.x, -vector.y,0);
        vertices[index+1].Normal = Vector3(-vector.y, vector.x,0);
        
         */
        
        float sin = sinf(particle->rotation);
        float cos = cosf(particle->rotation);
        
        vector.x = particle->size * cos - particle->size * sin;
        vector.y = particle->size * sin + particle->size * cos;
        
        vertices[index].Normal = Vector3(-vector.x, -vector.y,0);
        vertices[index+1].Normal = Vector3(vector.y, -vector.x,0);
        vertices[index+2].Normal = Vector3(vector.x, vector.y,0);
        vertices[index+3].Normal = Vector3(-vector.y, vector.x,0);
         
        index+=4;
    }
}