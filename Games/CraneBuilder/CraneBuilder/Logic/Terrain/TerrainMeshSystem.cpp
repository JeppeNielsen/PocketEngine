//
//  TerrainMeshSystem.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 03/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "TerrainMeshSystem.h"
#include "Triangulator.hpp"

void TerrainMeshSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Terrain>()->VerticesChanged += event_handler(this, &TerrainMeshSystem::VerticesChanged, object);
    changedTerrains.insert(object);
}

void TerrainMeshSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Terrain>()->VerticesChanged -= event_handler(this, &TerrainMeshSystem::VerticesChanged, object);
    auto it = changedTerrains.find(object);
    if (it!=changedTerrains.end()) {
        changedTerrains.erase(object);
    }
}

void TerrainMeshSystem::VerticesChanged(Terrain *terrain, Pocket::GameObject *object) {
    changedTerrains.insert(object);
}

void TerrainMeshSystem::Update(float dt) {
    for(GameObject* go : changedTerrains) {
        CreateMesh(go);
    }
    changedTerrains.clear();
}

void TerrainMeshSystem::CreateMesh(Pocket::GameObject *object) {
    Terrain* terrain = object->GetComponent<Terrain>();
    auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
    mesh.Clear();
    if (terrain->vertices.size()<2) return;
    
    Terrain::Vertices smoothedVertices = terrain->GetSmoothedVertices(terrain->vertices, 10);
    
    Terrain::Vertices normals = terrain->CalculateNormals(smoothedVertices);
    
    for(auto& layer : terrain->layers) {
        AddLayer(smoothedVertices, normals, mesh, layer);
    }

    {
        auto& centerMesh = object->Children()[0]->GetComponent<Mesh>()->GetMesh<Vertex>();
        centerMesh.Clear();

        Triangulator::IndiciesVector indicies;
        Triangulator::Triangulate(smoothedVertices, indicies);
        
        int startIndex = (int)centerMesh.vertices.size();
        for (int i=0; i<smoothedVertices.size(); i++) {
            Vertex v;
            v.TextureCoords = smoothedVertices[i]*0.2f;// + normals[i] * 0.4f + Vector2(0,-i)*0.06f;
            v.Position = smoothedVertices[i];
            centerMesh.vertices.push_back(v);
        }
        
        for (int i=0; i<indicies.size(); i++) {
            centerMesh.triangles.push_back((short)startIndex + indicies[i]);
        }
    }
}

Vector2 TerrainMeshSystem::GetVertex(int index, const Terrain::Vertices &vertices) {
    while (index<0) {
        index+=vertices.size();
    }
    return vertices[index % vertices.size()];
}

float TerrainMeshSystem::GetSlope(const Pocket::Vector2 &vector, const Terrain::Vertices &normals, int index) {
    while (index<0) {
        index+=normals.size();
    }
    return acosf(vector.Dot(normals[index % normals.size()]));
}

void TerrainMeshSystem::ModifyVertices(const Terrain::Vertices &vertices, const Terrain::Vertices &normals, float distance, Terrain::Vertices &output) {
    for (int i=0; i<vertices.size(); i++) {
        output[i] = vertices[i] - normals[i] * distance;
    }
}

void TerrainMeshSystem::AddLayer(const Terrain::Vertices &vertices, const Terrain::Vertices &normals, VertexMesh<Pocket::Vertex> &mesh, const Terrain::Layer &layer) {
    
    Terrain::Vertices outer;
    outer.resize(vertices.size());
    ModifyVertices(vertices, normals, layer.outerDepth, outer);
    
    Terrain::Vertices inner;
    inner.resize(vertices.size());
    ModifyVertices(vertices, normals, layer.innerDepth, inner);
    
    int startTriangle = (int)mesh.vertices.size();
    const Vector2 lightNormal = Vector2(0.5f,-1).Normalized();
    
    float u = 0;
    for (int i=0; i<=vertices.size(); i++) {
        
        int index = i==vertices.size() ? 0 : i;
        
        float dot = MathHelper::Clamp(lightNormal.Dot(normals[index]), 0, 1);
        
        Vertex outerVertex;
        outerVertex.Position = outer[index];
        outerVertex.TextureCoords = { u, layer.outerV };
        outerVertex.Color = !layer.isBevel ? Colour::White() : Colour::Lerp(Colour::White(0.3f), Colour::Black(0.3f), dot);
        
        if (layer.useDirection) {
            float directionInfluence = (MathHelper::Clamp(layer.direction.Dot(normals[index]),layer.directionAmount,1.0f) - layer.directionAmount) / (1-layer.directionAmount);
            outerVertex.Color = Colour::Lerp(Colour::White(0.0f), outerVertex.Color, directionInfluence);
        }
        
        outerVertex.Color = outerVertex.Color * layer.color;
        mesh.vertices.push_back(outerVertex);
        
        Vertex innerVertex;
        innerVertex.Position = inner[index];
        innerVertex.TextureCoords = { u, layer.innerV };
        innerVertex.Color = !layer.isBevel ? Colour::White() : Colour::Lerp(Colour::White(0.0f), Colour::Black(0.0f), dot);

        if (layer.useDirection) {
            float directionInfluence = (MathHelper::Clamp(layer.direction.Dot(normals[index]),layer.directionAmount,1.0f) - layer.directionAmount) / (1-layer.directionAmount);
            innerVertex.Color = Colour::Lerp(Colour::White(0.0f), innerVertex.Color, directionInfluence);
        }
        
        innerVertex.Color = innerVertex.Color * layer.color;
        mesh.vertices.push_back(innerVertex);
        
        if (i<vertices.size()) {
            Vector2 mid = (outer[i] + inner[i]) * 0.5f;
            Vector2 midNext = (outer[i==vertices.size() - 1 ? 0 : i + 1] + inner[i==vertices.size() - 1 ? 0 : i + 1]) * 0.5f;
            
            float length = (mid - midNext).Length();
            u += length * layer.textureScale;
        
            int outerIndex = startTriangle + i * 2;
            int innerIndex = startTriangle + i * 2 + 1;
            
            int outerIndexNext = outerIndex + 2;
            int innerIndexNext = innerIndex + 2;
            
            mesh.triangles.push_back(outerIndex);
            mesh.triangles.push_back(outerIndexNext);
            mesh.triangles.push_back(innerIndex);
            
            
            mesh.triangles.push_back(innerIndexNext);
            mesh.triangles.push_back(innerIndex);
            mesh.triangles.push_back(outerIndexNext);
        }
    }
    
}