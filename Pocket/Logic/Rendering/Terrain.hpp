//
//  Terrain.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/22/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once

#include "GameWorld.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Colour.hpp"
#include <vector>
#include "TextureComponent.hpp"

namespace Pocket {
    Component(Terrain)
public:
    
     void Reset();
     int meshPatchSize;
     int xSegments;
     int zSegments;
     float sizeRadius;
     Vector2 size;
     float lod;
     float uvScale;
     int maxLevels;
     float minHeight;
     float maxHeight;
    Vector2 segmentSize;
    Vector2 segmentFloat;
    
    TextureComponent* Textures[4];
    
    struct Node {
        float height;
        Vector3 Normal;
        Colour texture;
    };
    
    float GetHeight(float x, float z, Vector3& normal, Colour& texture) const;
    
    void AddHill(int x, int z, int radius, float height);
    
    void SetHeight(int x, int z, float height);
    
    void Smooth(int iterations);
    
    void Smooth(int iterations, int startX, int startZ, int width, int depth);
    
    void RandomBumps(float min, float max);
    
    void RandomBumps(int x, int z, int width, int depth, float min, float max);
    
    bool IntersectsRay(const Ray &localRay, float *pickDistance, float *barycentricU, float *barycentricV, size_t* triangleIndex, Vector3* normal);
    
    void TextureCircle(int x, int z, int radius, float t1, float t2, float t3, float t4);
    
    void TextureSet(int x, int z, float t1, float t2, float t3, float t4);
    
private:
    
    bool IsCoordValid(int xPos, int zPos);
    void CalculateMaxLevels();
    void CreateData(int xSeg, int zSeg);
    
    int ClampValue(int x, int size);
    
    typedef std::vector<Node> Nodes;
    Nodes nodes;
    
    void UpdateNormals(int x, int z, int width, int depth);
    void UpdateNormal(int x, int z);
    
    struct IntersectData {
        float minDistance;
        unsigned int triangleIndex;
        float barycentricU, barycentricV;
        bool hit;
        Vector3 v0, v1, v2;
    };
    
    void IntersectQuad(int x, int z, int width, int depth, int level, const Ray& ray, IntersectData& intersectData);
    
    
};
    
}