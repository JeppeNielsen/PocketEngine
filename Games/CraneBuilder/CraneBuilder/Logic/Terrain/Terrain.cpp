//
//  Terrain.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 03/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Terrain.h"


void Terrain::Reset() {
    vertices.clear();
}


Terrain::Vertices Terrain::CalculateNormals(const Terrain::Vertices& vertices) {
    Vertices normals;
    normals.resize(vertices.size());
    
    for (int i=0; i<vertices.size(); i++) {
        const Vector2& vertex = vertices[i];
        const Vector2& next = vertices[i==vertices.size()-1 ? 0 : i + 1];
        const Vector2& prev = vertices[i==0 ? vertices.size()-1 : i - 1];
        
        Vector2 toNextNormal(next.y - vertex.y, vertex.x - next.x);
        Vector2 toPrevNormal(vertex.y - prev.y, prev.x - vertex.x);
        toNextNormal.Normalize();
        toPrevNormal.Normalize();
        
        normals[i] = (toNextNormal + toPrevNormal).Normalized();
    }
    return normals;
}

Terrain::Vertices Terrain::GetSmoothedVertices(const Terrain::Vertices& vertices, int segments) {
    Vertices tangentsForward;
    tangentsForward.resize(vertices.size());
    
    Vertices tangentsBackward;
    tangentsBackward.resize(vertices.size());
    
    const float amount = 0.125f;
    
    for (int i=0; i<vertices.size(); i++) {
        const Vector2& vertex = vertices[i];
        const Vector2& next = vertices[i==vertices.size()-1 ? 0 : i + 1];
        const Vector2& prev = vertices[i==0 ? vertices.size()-1 : i - 1];
        
        Vector2 toNext = (next - vertex);
        Vector2 toPrev = (prev - vertex);
        
        float toNextLen = toNext.Length();
        float toPrevLen = toPrev.Length();
        
        toNext /= toNextLen;
        toPrev /= toPrevLen;
        
        tangentsForward[i] = vertex + (-toPrev + toNext).Normalized() * toNextLen * amount;
        tangentsBackward[i] = vertex + (toPrev - toNext).Normalized() * toPrevLen * amount;
    }
    
    Terrain::Vertices smoothedVertices;
    
    for (int i=0; i<vertices.size(); i++) {
        int nextIndex = i==vertices.size()-1 ? 0 : i + 1;
        float dt = 1.0f / segments;
        for (int s=0; s<segments; s++) {
            smoothedVertices.push_back(Vector2::Bezier(vertices[i], tangentsForward[i], vertices[nextIndex], tangentsBackward[nextIndex], s * dt));
        }
    }

    return smoothedVertices;
}

void Terrain::CheckForChange() {
    bool wasChanged = false;
    
    if (vertices.size()!=prevVertices.size()) {
        wasChanged = true;
    } else {
        for (int i=0; i<vertices.size(); i++) {
            if (!vertices[i].EqualEpsilon(prevVertices[i])) {
                wasChanged = true;
                break;
            }
        }
    }

    prevVertices = vertices;

    if (wasChanged) {
        VerticesChanged(this);
    }
}
