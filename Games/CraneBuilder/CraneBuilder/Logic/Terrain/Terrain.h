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

struct Terrain {
    Terrain();
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
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(outerDepth);
        TYPE_FIELD(innerDepth);
        TYPE_FIELD(textureScale);
        TYPE_FIELD(outerV);
        TYPE_FIELD(innerV);
        TYPE_FIELD(isBevel);
        TYPE_FIELD(useDirection);
        TYPE_FIELD(direction);
        TYPE_FIELD(directionAmount);
        TYPE_FIELD(color);
        TYPE_FIELDS_END
    };

    typedef std::vector<Layer> Layers;
    Layers layers;

    Vertices CalculateNormals(const Terrain::Vertices& vertices);

    void CheckForChange();

    Event<Terrain*> VerticesChanged;

    TYPE_FIELDS_BEGIN
    TYPE_FIELD(vertices)
    TYPE_FIELD(layers)
    TYPE_FIELDS_END
};