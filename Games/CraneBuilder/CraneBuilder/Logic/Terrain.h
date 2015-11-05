//
//  Terrain.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 03/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Vector2.hpp"
#include <vector>
#include "Mesh.hpp"

using namespace Pocket;

Component(Terrain)
    void Reset();
    typedef std::vector<Vector2> Vertices;
    Vertices vertices;

    float startDepth;

    Vertices GetSmoothedVertices(const Terrain::Vertices& vertices, int segments);

    struct Layer {
        float outerDepth;
        float innerDepth;
        float textureScale;
        float outerV;
        float innerV;
        bool isBevel;
    };

    typedef std::vector<Layer> Layers;
    Layers layers;

    Vertices CalculateNormals(const Terrain::Vertices& vertices);

    Pointer<Mesh> centerMesh;
    Pointer<Mesh> bevelMesh;
};