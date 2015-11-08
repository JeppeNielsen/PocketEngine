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
    Vertices prevVertices;

    float startDepth;

    Vertices GetSmoothedVertices(const Terrain::Vertices& vertices, int segments);

    struct Layer {
        Layer() {
            outerDepth = 0;
            innerDepth = 0;
            textureScale = 1.0f;
            outerV = 0;
            innerV = 1.0f;
            isBevel = false;
            useDirection = false;
            direction = Vector2(0,-1);
            directionAmount = 0.0f;
            color = Colour::White();
        }
    
        float outerDepth;
        float innerDepth;
        float textureScale;
        float outerV;
        float innerV;
        bool isBevel;
        bool useDirection;
        Vector2 direction;
        float directionAmount;
        Colour color;
    };

    typedef std::vector<Layer> Layers;
    Layers layers;

    Vertices CalculateNormals(const Terrain::Vertices& vertices);

    Pointer<Mesh> centerMesh;
    Pointer<Mesh> bevelMesh;
    Pointer<Mesh> platformMesh;

    void CheckForChange();

    Event<Terrain*> VerticesChanged;
};