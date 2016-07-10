//
//  TerrainMeshSystem.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 03/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Terrain.h"
#include "Mesh.hpp"
#include <set>

using namespace Pocket;

struct TerrainMeshSystem: public GameSystem<Terrain, Mesh> {
public:
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
    void CreateMesh(GameObject* object);
    void ModifyVertices(const Terrain::Vertices& vertices, const Terrain::Vertices& normals, float distance, Terrain::Vertices& output);
    void AddLayer(const Terrain::Vertices& vertices, const Terrain::Vertices& normals, VertexMesh<Vertex>& mesh, const Terrain::Layer& layer);

    float GetSlope(const Vector2 &vector, const Terrain::Vertices& normals, int index);
    Vector2 GetVertex(int index, const Terrain::Vertices& vertices);

    void VerticesChanged(Terrain* terrain, GameObject* object);

    typedef std::set<GameObject*> ChangedTerrains;
    ChangedTerrains changedTerrains;
};