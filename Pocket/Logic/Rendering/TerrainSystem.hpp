//
//  TerrainSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/22/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once

#include "GameWorld.hpp"
#include "Terrain.hpp"
#include "Mesh.hpp"
#include <map>

namespace Pocket {
    class TerrainSystem : public GameSystem {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        
    private:
        void CreateMesh(Terrain* terrain, Mesh* mesh);
        void CalcBoundingBox( DirtyProperty<Mesh*, BoundingBox>::EventData& eventData, GameObject* object );
    
        typedef std::map<Mesh*, Terrain*> MeshToTerrain;
        MeshToTerrain meshToTerrain;
        
        class TerrainIntersector : public MeshIntersector {
        public:
            TerrainSystem* system;
            bool IntersectsRay(Mesh& mesh, const Ray& ray,
                                       float* pickDistance, float* barycentricU, float* barycentricV,
                                       size_t* triangleIndex, Vector3* normal);
        };
        TerrainIntersector intersector;
    };
}