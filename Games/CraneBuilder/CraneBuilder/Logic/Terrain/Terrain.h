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

    struct Layer : public ISerializable{
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
        
        SERIALIZE_FIELDS_BEGIN
        SERIALIZE_FIELD(outerDepth);
        SERIALIZE_FIELD(innerDepth);
        SERIALIZE_FIELD(textureScale);
        SERIALIZE_FIELD(outerV);
        SERIALIZE_FIELD(innerV);
        SERIALIZE_FIELD(isBevel);
        SERIALIZE_FIELD(useDirection);
        SERIALIZE_FIELD(direction);
        SERIALIZE_FIELD(directionAmount);
        SERIALIZE_FIELD(color);
        SERIALIZE_FIELDS_END
    };

    typedef std::vector<Layer> Layers;
    Layers layers;

    Vertices CalculateNormals(const Terrain::Vertices& vertices);

    void CheckForChange();

    Event<Terrain*> VerticesChanged;

    SERIALIZE_FIELDS_BEGIN
    SERIALIZE_FIELD(vertices)
    SERIALIZE_FIELD(layers)
    SERIALIZE_FIELDS_END
};