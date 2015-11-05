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

using namespace Pocket;

SYSTEM(TerrainMeshSystem, Terrain, Mesh)
public:
    void ObjectAdded(GameObject* object);
private:
    void ModifyVertices(const Terrain::Vertices& vertices, const Terrain::Vertices& normals, float distance, Terrain::Vertices& output);
    void AddLayer(const Terrain::Vertices& vertices, const Terrain::Vertices& normals, VertexMesh<Vertex>& mesh, const Terrain::Layer& layer);
};