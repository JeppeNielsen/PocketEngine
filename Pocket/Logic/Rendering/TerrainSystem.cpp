//
//  TerrainSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/22/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "TerrainSystem.hpp"

using namespace Pocket;

void TerrainSystem::Initialize() {
    AddComponent<Terrain>();
    AddComponent<Mesh>();
    intersector.system = this;
}

void TerrainSystem::ObjectAdded(GameObject* object) {
    Mesh* mesh = object->GetComponent<Mesh>();
    mesh->LocalBoundingBox.Method.Clear();
    mesh->LocalBoundingBox.Method += event_handler(this, &TerrainSystem::CalcBoundingBox, object);
    mesh->customIntersector = &intersector;
    meshToTerrain[mesh]=object->GetComponent<Terrain>();
    CreateMesh(object->GetComponent<Terrain>(), mesh);
}

void TerrainSystem::ObjectRemoved(GameObject* object) {
    Mesh* mesh = object->GetComponent<Mesh>();
    mesh->RevertDefaultCalcBoundingBox();
    mesh->customIntersector = 0;
    
    MeshToTerrain::iterator it = meshToTerrain.find(mesh);
    if (it!=meshToTerrain.end()) {
        meshToTerrain.erase(it);
    }
}

void TerrainSystem::CreateMesh(Terrain *terrain, Mesh *mesh) {
    
    int meshPatchSize = terrain->meshPatchSize;
    
    Mesh::VerticesList& vertices = mesh->Vertices();
    Mesh::TrianglesList& triangles = mesh->Triangles();
    
    vertices.resize(meshPatchSize * meshPatchSize + meshPatchSize * 4);
    triangles.resize((meshPatchSize - 1) * (meshPatchSize - 1) * 6 + (meshPatchSize - 1) * 4 * 6);
    
    float delta = 1.0f / (meshPatchSize - 1);
    
    float px = 0;
    float pz = 0;
    
    int i = 0;
    int d = 0;
    for (int z = 0; z < meshPatchSize; z++)
    {
        pz = z * delta;
        
        for (int x = 0; x < meshPatchSize; x++)
        {
            px = x * delta;
            
            Vertex& vertex = vertices[i];
            vertex.Position = Vector3(px, 1, pz);
            vertex.Normal = Vector3(0,1,0);
            vertex.TextureCoords = Vector2(0,0);
            
            if (x < meshPatchSize - 1 && z < meshPatchSize - 1)
            {
                triangles[d++] = (short)(i);
                triangles[d++] = (short)(i + 1+ meshPatchSize);
                triangles[d++] = (short)(i + 1);
                
                triangles[d++] = (short)(i);
                triangles[d++] = (short)(i + meshPatchSize);
                triangles[d++] = (short)(i + 1 + meshPatchSize);
            }
            
            i++;
        }
    }
    
    for (int c = 0; c < meshPatchSize; c++)
    {
        px = c * delta;

        Vertex& vertex = vertices[i];
        vertex.Position = Vector3(px, -1, 0);
        vertex.Normal = Vector3(0,1,0);
        vertex.TextureCoords = Vector2(0,0);
        
        if (c < meshPatchSize - 1)
        {
            triangles[d++] = (short)(i);
            triangles[d++] = (short)(c + 1);
            triangles[d++] = (short)(i + 1);
            
            triangles[d++] = (short)(i);
            triangles[d++] = (short)(c);
            triangles[d++] = (short)(c + 1);
        }
        i++;
    }
    
    int b = meshPatchSize * meshPatchSize - meshPatchSize;
    
    for (int c = 0; c < meshPatchSize; c++)
    {
        
        px = c * delta;
        
        Vertex& vertex = vertices[i];
        vertex.Position = Vector3(px, -1, 1);
        vertex.Normal = Vector3(0,1,0);
        vertex.TextureCoords = Vector2(0,0);
        
        if (c < meshPatchSize - 1)
        {
            
            triangles[d++] = (short)(b + c);
            triangles[d++] = (short)(i + 1);
            triangles[d++] = (short)(b + c + 1);
            
            triangles[d++] = (short)(b + c);
            triangles[d++] = (short)(i);
            triangles[d++] = (short)(i + 1);
            
        }
        i++;
    }
    
    for (int c = 0; c < meshPatchSize; c++)
    {
        pz = c * delta;
        
        Vertex& vertex = vertices[i];
        vertex.Position = Vector3(0, -1, pz);
        vertex.Normal = Vector3(0,1,0);
        vertex.TextureCoords = Vector2(0,0);
        
        if (c < meshPatchSize - 1)
        {
            
            triangles[d++] = (short)(i);
            triangles[d++] = (short)(c * meshPatchSize + meshPatchSize);
            triangles[d++] = (short)(c * meshPatchSize);
            
            triangles[d++] = (short)(i);
            triangles[d++] = (short)(i + 1);
            triangles[d++] = (short)(c * meshPatchSize + meshPatchSize);
        }
        
        i++;
    }
    
    for (int c = 0; c < meshPatchSize; c++)
    {
        pz = c * delta;
        
        Vertex& vertex = vertices[i];
        vertex.Position = Vector3(1, -1, pz);
        vertex.Normal = Vector3(0,1,0);
        vertex.TextureCoords = Vector2(0,0);
        
        if (c < meshPatchSize - 1)
        {
            triangles[d++] = (short)(meshPatchSize - 1 + c * meshPatchSize);
            triangles[d++] = (short)(i + 1);
            triangles[d++] = (short)(i);
            
            triangles[d++] = (short)(meshPatchSize - 1 + c * meshPatchSize);
            triangles[d++] = (short)(meshPatchSize - 1 + c * meshPatchSize + meshPatchSize);
            triangles[d++] = (short)(i + 1);
        }
        
        i++;
    }
}

void TerrainSystem::CalcBoundingBox( DirtyProperty<Mesh*, BoundingBox>::EventData& eventData, GameObject* object )
{
    Terrain* terrain = object->GetComponent<Terrain>();
    BoundingBox& box = *eventData.Value;
    Vector3 min = Vector3(0,terrain->minHeight,0);
	Vector3 max = Vector3(terrain->size.x, terrain->maxHeight, terrain->size.y);
    box.center = (min + max) * 0.5f;
	box.extends = (max - min);
}

bool TerrainSystem::TerrainIntersector::IntersectsRay(Mesh& mesh, const Ray &ray, float *pickDistance, float *barycentricU, float *barycentricV, size_t* triangleIndex, Vector3* normal) {
    Terrain* terrain = system->meshToTerrain[&mesh];
    return terrain->IntersectsRay(ray, pickDistance, barycentricU, barycentricV, triangleIndex, normal);
}
