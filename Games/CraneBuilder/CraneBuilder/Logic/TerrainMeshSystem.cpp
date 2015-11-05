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
    Terrain* terrain = object->GetComponent<Terrain>();
    auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
    auto& bevelMesh = terrain->bevelMesh->GetMesh<Vertex>();
    
    Terrain::Vertices smoothedVertices = terrain->GetSmoothedVertices(terrain->vertices, 10);
    
    Terrain::Vertices normals = terrain->CalculateNormals(smoothedVertices);
    
    for(auto& layer : terrain->layers) {
        AddLayer(smoothedVertices, normals, layer.isBevel ? bevelMesh : mesh, layer);
    }

    auto& centerMesh = terrain->centerMesh->GetMesh<Vertex>();

    Triangulator::IndiciesVector indicies;
    Triangulator::Triangulate(smoothedVertices, indicies);
    
    int startIndex = (int)centerMesh.vertices.size();
    for (int i=0; i<smoothedVertices.size(); i++) {
        Vertex v;
        v.TextureCoords = smoothedVertices[i]*0.2f + normals[i] * 0.4f + Vector2(0,-i)*0.06f;
        v.Position = smoothedVertices[i];
        centerMesh.vertices.push_back(v);
    }
    
    for (int i=0; i<indicies.size(); i++) {
        centerMesh.triangles.push_back((short)startIndex + indicies[i]);
    }
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
    for (int i=0; i<vertices.size(); i++) {
        
        float dot = lightNormal.Dot(normals[i]);
    
        Vertex outerVertex;
        outerVertex.Position = outer[i];
        outerVertex.TextureCoords = { u, layer.outerV };
        outerVertex.Color = !layer.isBevel ? Colour::White() : Colour::Lerp(Colour::White(0.3f), Colour::Black(0.3f), dot);
        mesh.vertices.push_back(outerVertex);
        
        Vertex innerVertex;
        innerVertex.Position = inner[i];
        innerVertex.TextureCoords = { u, layer.innerV };
        innerVertex.Color = !layer.isBevel ? Colour::White() : Colour::Lerp(Colour::White(0.0f), Colour::Black(0.0f), dot);
        
        
        mesh.vertices.push_back(innerVertex);
        
        Vector2 mid = (outer[i] + inner[i]) * 0.5f;
        Vector2 midNext = (outer[i==vertices.size()-1 ? 0 : i + 1] + inner[i==vertices.size()-1 ? 0 : i + 1]) * 0.5f;
        
        float length = (mid - midNext).Length();
        u += length * layer.textureScale;
    
        int outerIndex = startTriangle + i * 2;
        int innerIndex = startTriangle + i * 2 + 1;
        
        int outerIndexNext = i == vertices.size() - 1 ? startTriangle : outerIndex + 2;
        int innerIndexNext = i == vertices.size() - 1 ? startTriangle+1 : innerIndex + 2;
        
        
        mesh.triangles.push_back(outerIndex);
        mesh.triangles.push_back(outerIndexNext);
        mesh.triangles.push_back(innerIndex);
        
        
        mesh.triangles.push_back(innerIndexNext);
        mesh.triangles.push_back(innerIndex);
        mesh.triangles.push_back(outerIndexNext);
        
    }
    
}