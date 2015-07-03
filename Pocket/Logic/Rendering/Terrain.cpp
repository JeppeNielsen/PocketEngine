//
//  Terrain.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/22/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Terrain.hpp"
#include <cmath>
#include "MathHelper.hpp"
#include "Ray.hpp"

using namespace Pocket;

void Terrain::Reset() {
    size = Vector2(128,128);
    lod = 30.0f;
    uvScale = 32.0f;
    meshPatchSize = 33;
    minHeight = 0;
    maxHeight = 64.0f;
    CreateData(512,512);
    CalculateMaxLevels();
}

void Terrain::CalculateMaxLevels() {
    int iPow2 = (xSegments) / (meshPatchSize - 1);
    
    maxLevels = 0;
    for (int i = 1; i < iPow2; i *= 2)
    {
        maxLevels++;
    }
}

void Terrain::CreateData(int xSeg, int zSeg) {
    xSegments = xSeg;
    zSegments = zSeg;
    size_t count = (xSegments + 1) * (zSegments + 1);
    nodes.resize(count);
    for (size_t i = 0; i<count; ++i) {
        nodes[i].height = 0;
        nodes[i].Normal = Vector3(0,1,0);
        nodes[i].texture = Colour((Colour::Component)0,0,0,0);
    }
    
    
    segmentSize = Vector2(size.x / xSegments, size.y / zSegments );
    segmentFloat = Vector2(xSeg, zSeg);
    
}

float Terrain::GetHeight(float x, float z, Vector3& normal, Colour& texture) const {
    
    int xpos = (int)(x * segmentFloat.x);
    int zpos = (int)(z * segmentFloat.y);
    
    int segment = xpos + zpos * xSegments;
    
    const Node& a = nodes[segment];
    
    //const Node& d = nodes[segment + 1 + xSegments];
            
    //float n1y = a.height - b.height;
    //float n2y = a.height - c.height;
    
    //n1.z = 0;
    //n2.x = 0;
    
    //n1.x = segmentSize.x;
    //n2.z = segmentSize.y;
    
    
    //normal.x = segmentSize.y * n1y;
    //normal.y = segmentSize.y * segmentSize.x;
    //normal.z = n2y * segmentSize.x;
    
    
    //n2.Cross(n1, normal);
    
    //Vector3 n3 = Vector3(segmentSize.x,d.height - c.height, 0);
    //Vector3 n4 = Vector3(0,d.height - b.height, segmentSize.y);
    
    //normal += n4.Cross(n3);
    
    
    
    //works but a bit crude:
    /*
    const Node& b = nodes[segment + 1];
    const Node& c = nodes[segment + xSegments];
    
    float n1y = a.height - b.height;
    float n2y = a.height - c.height;
    
    normal.x = segmentSize.y * n1y;
    normal.y = segmentSize.y * segmentSize.x;
    normal.z = n2y * segmentSize.x;
    */
     
    normal = a.Normal;
    
    texture = a.texture;
    
    return a.height;
}

/*
float Terrain::GetHeight(float x, float z, Vector3& normal) const {
    
    float xworld = x * (float)xSegments;
    float zworld = z * (float)zSegments;
    
    int xpos = (int)xworld;
    xpos = (xpos - ( xpos > xworld ));
    
    int zpos = (int)zworld;
    zpos = (zpos - ( zpos > zworld ));
    
    //int xpos = (int)floorf(xworld);
    //int zpos = (int)floorf(zworld);
    if (xpos<0) { xpos = 0; } else if (xpos>=xSegments) { xpos = xSegments; }
    if (zpos<0) { zpos = 0; } else if (zpos>=zSegments) { zpos = zSegments; }
    
    const Node& a = nodes[xpos + zpos * xSegments];
    const Node& b = nodes[xpos + zpos * xSegments + 1];
    
    const Node& c = nodes[xpos + zpos * xSegments + xSegments];
    const Node& d = nodes[xpos + zpos * xSegments + 1 + xSegments];
    
    float ix = xworld - xpos;
    float iz = zworld - zpos;
    
    float ab = a.height + (b.height - a.height) * ix;
    float cd = c.height + (d.height - c.height) * ix;
    
    Vector3 n1 = Vector3(segmentSize.x,b.height - a.height, 0);
    Vector3 n2 = Vector3(0,c.height - a.height, segmentSize.y);
    
    normal = n2.Cross(n1);
    
    Vector3 n3 = Vector3(segmentSize.x,d.height - c.height, 0);
    Vector3 n4 = Vector3(0,d.height - b.height, segmentSize.y);
    
    normal += n4.Cross(n3);
    normal.Normalize();
    
    
    return ab + (cd - ab) * iz;;
}
 */

bool Terrain::IsCoordValid(int xPos, int zPos)
{
    return (xPos >= 0 && zPos >= 0 && xPos < xSegments && zPos < zSegments);
}

void Terrain::AddHill(int xPos, int zPos, int radius, float height)
{
    float radiusFloat = (float)radius;
    for (int z = -radius; z <= radius; z++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            int xx = xPos + x;
            int zz = zPos + z;
            
            if (IsCoordValid(xx, zz))
            {
                float rad = sqrtf(x * x + z * z);
                
                if (rad <= radiusFloat)
                {
                    float radPercent = rad / radiusFloat;
                    
                    float sinHeight = cosf(radPercent * M_PI_2);
                    
                    //sinHeight = (1 - radPercent);
                    
                    //sinHeight = 1 - HermiteInterpolate(radPercent, 0, 1, 1, 1, 0.625f, 0);
                    
                    SetHeight(xx, zz, nodes[xx + zz * xSegments].height + sinHeight * height);
                }
            }
            
        }
    }
    UpdateNormals(xPos - radius, zPos - radius, radius + radius, radius + radius);
}

void Terrain::SetHeight(int x, int z, float height)
{
    nodes[x + z * xSegments].height = height;
    
    if (height > maxHeight) maxHeight = height;
    if (height < minHeight) minHeight = height;
}

void Terrain::Smooth(int iterations) {
    Smooth(iterations, 0, 0, xSegments, zSegments);
}   

void Terrain::Smooth(int iterations, int startX, int startZ, int width, int depth)
{
    int x2 = ClampValue(startX + width, xSegments - 1);
    int z2 = ClampValue(startZ + depth, zSegments - 1);
    
    startX = ClampValue(startX, xSegments - 1);
    startZ = ClampValue(startZ, zSegments - 1);
    
    width = x2 - startX;
    depth = z2 - startZ;
    
    for (int i = 0; i < iterations; i++)
    {
        for (int z = startZ; z < startZ + depth; z++)
        {
            for (int x = startX; x < startX + width; x++)
            {
                float height = 0;
                int count = 0;
                
                for (int xx = -1; xx <= 1; xx++)
                {
                    for (int zz = -1; zz <= 1; zz++)
                    {
                        int xxx = x + xx;
                        int zzz = z + zz;
                        
                        if (xxx >= 0 && zzz >= 0 && xxx < xSegments && zzz < zSegments)
                        {
                            height += nodes[xxx + zzz * xSegments].height;;
                            count++;
                        }
                    }
                }
                
                height /= count;
                
                SetHeight(x, z, height);
            }
        }
        
    }
    
    UpdateNormals(startX, startZ, width, depth);
}

int Terrain::ClampValue(int x, int size)
{
    return x < 0 ? 0 : (x>size ? size : x);
}

void Terrain::RandomBumps(float min, float max) {
    RandomBumps(0, 0, xSegments, zSegments, min, max);
}

void Terrain::RandomBumps(int x, int z, int width, int depth, float min, float max)
{
    int x2 = ClampValue(x + width, xSegments - 1);
    int z2 = ClampValue(z + depth, zSegments - 1);
    
    x = ClampValue(x, xSegments - 1);
    z = ClampValue(z, zSegments - 1);
    
    width = x2 - x;
    depth = z2 - z;
    
    for (int zz = 0; zz < depth; zz++)
    {
        for (int xx = 0; xx < width; xx++)
        {
            float am = MathHelper::Random();
            
            SetHeight(x + xx, z + zz, nodes[x + xx + (z + zz) * xSegments].height + min + (max - min) * am);
        }
    }
    
    UpdateNormals(x, z, width, depth);
}

void Terrain::UpdateNormals(int x, int z, int width, int depth) {
    
    x = ClampValue(x, xSegments - 1);
    z = ClampValue(z, zSegments - 1);
    
    width += x;
    depth += z;
    
    width = ClampValue(width, xSegments - 1);
    depth = ClampValue(depth, zSegments - 1);
    
    for (int zz = z; zz<depth; ++zz) {
        for (int xx = x; xx<width; ++xx) {
            UpdateNormal(xx, zz);
        }
    }
}

void Terrain::UpdateNormal(int x, int z) {
    int index = x + z * xSegments;
    Node& node = nodes[index];
    
    
    if (z==0) {
        Vector3 up(0, nodes[index + xSegments].height - node.height, segmentSize.y);
        if (x==0) {
            Vector3 left(segmentSize.x, nodes[index + 1].height - node.height,0);
            up.Cross(left, node.Normal);
        } else if (x<xSegments - 1) {
            Vector3 left(segmentSize.x, nodes[index + 1].height - node.height,0);
            Vector3 right(-segmentSize.x, nodes[index - 1].height - node.height,0);
            up.Cross(left, node.Normal);
            node.Normal += right.Cross(up);
        } else {
            Vector3 right(-segmentSize.x, nodes[index - 1].height - node.height,0);
            right.Cross(up, node.Normal);
        }
    } else if (z < zSegments - 1) {
        
        Vector3 up(0, nodes[index + xSegments].height - node.height, segmentSize.y);
        Vector3 down(0, nodes[index - xSegments].height - node.height, -segmentSize.y);
        
        if (x==0) {
            Vector3 left(segmentSize.x, nodes[index + 1].height - node.height,0);
            up.Cross(left, node.Normal);
            node.Normal += left.Cross(down);
        } else if (x<xSegments - 1) {
            Vector3 left(segmentSize.x, nodes[index + 1].height - node.height,0);
            Vector3 right(-segmentSize.x, nodes[index - 1].height - node.height,0);
            up.Cross(left, node.Normal);
            node.Normal += right.Cross(up);
            node.Normal += down.Cross(right);
            node.Normal += left.Cross(down);
        } else {
            Vector3 right(-segmentSize.x, nodes[index - 1].height - node.height,0);
            right.Cross(up, node.Normal);
            node.Normal += down.Cross(right);
        }
    } else {
        
        Vector3 down(0, nodes[index - xSegments].height - node.height, -segmentSize.y);
        
        if (x==0) {
            Vector3 left(segmentSize.x, nodes[index + 1].height - node.height,0);
            left.Cross(down, node.Normal);
        } else if (x<xSegments - 1) {
            Vector3 left(segmentSize.x, nodes[index + 1].height - node.height,0);
            Vector3 right(-segmentSize.x, nodes[index - 1].height - node.height,0);
            left.Cross(down, node.Normal);
            node.Normal += down.Cross(right);
        } else {
            Vector3 right(-segmentSize.x, nodes[index - 1].height - node.height,0);
            down.Cross(right, node.Normal);
        }
    }
}

void Terrain::TextureCircle(int xPos, int zPos, int radius, float t1, float t2, float t3, float t4) {
    
    float radiusFloat = (float)radius;
    for (int z = -radius; z <= radius; z++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            int xx = xPos + x;
            int zz = zPos + z;
            
            if (IsCoordValid(xx, zz))
            {
                float rad = sqrtf(x * x + z * z);
                
                if (rad <= radiusFloat)
                {
                    float radPercent = rad / radiusFloat;
                    
                    float sinHeight = cosf(radPercent * M_PI_2);
                    
                    //float sinHeight = (1 - radPercent);
                    //sinHeight = 1 - HermiteInterpolate(radPercent, 0, 1, 1, 1, 0.625f, 0);
                    
                    TextureSet(xx,zz, t1 * sinHeight, t2 * sinHeight, t3 * sinHeight, t4 * sinHeight);
                    
                }
            }
            
        }
    }
}

void Terrain::TextureSet(int x, int z, float t1, float t2, float t3, float t4) {
    
    Node& node =  nodes[x + z * xSegments];
    
    float n1 = node.texture.R();
    float n2 = node.texture.G();
    float n3 = node.texture.B();
    float n4 = node.texture.A();
    
    n1 += t1;
    n2 += t2;
    n3 += t3;
    n4 += t4;
    
    if (n1<0) { n1 = 0; } else if (n1>1) { n1 = 1.0f; }
    if (n2<0) { n2 = 0; } else if (n2>1) { n2 = 1.0f; }
    if (n3<0) { n3 = 0; } else if (n3>1) { n3 = 1.0f; }
    if (n4<0) { n4 = 0; } else if (n4>1) { n4 = 1.0f; }
    
    node.texture = Colour(n1, n2, n3, n4);
}

bool Terrain::IntersectsRay(const Ray &localRay, float *pickDistance, float *barycentricU, float *barycentricV, size_t* triangleIndex, Vector3* normal) {
    
    IntersectData intersectData;
    intersectData.minDistance = 10000000.0f;
    intersectData.hit = false;
    
    IntersectQuad(0, 0, xSegments, zSegments, 8, localRay, intersectData);
    
    if (intersectData.hit) {
    
        *pickDistance = intersectData.minDistance;
        *barycentricU = intersectData.barycentricU;
        *barycentricV = intersectData.barycentricV;
        *triangleIndex = intersectData.triangleIndex;
        
        Vector3 tan0 = intersectData.v1 - intersectData.v0;
        Vector3 tan1 = intersectData.v2 - intersectData.v0;
        
        Vector3 n = tan0.Cross(tan1);
        n.Normalize();
        
        *normal = n;
        
        return true;
    } else {
        return false;
    }
}

void Terrain::IntersectQuad(int x, int z, int width, int depth, int level, const Pocket::Ray &ray, Pocket::Terrain::IntersectData &intersectData) {
    
    int widthHalf = width / 2;
    int depthHalf = depth / 2;
    
    BoundingBox box(Vector3((x + widthHalf) * segmentSize.x, (minHeight + maxHeight) * 0.5f, (z + depthHalf) * segmentSize.y),
                    Vector3(width * segmentSize.x, maxHeight - minHeight, depth * segmentSize.y));
    
    if (!ray.Intersect(box)) return;
    
    if (level==0) {
        
        float u,v;
        float distance;
        
        int xMax = x + width;
        int zMax = z + depth;
        if (xMax>xSegments-2) xMax = xSegments - 2;
        if (zMax>zSegments-2) zMax = zSegments - 2;
        
        Vector3 v0, v1, v2;
        
        for (int zz=z; zz<zMax; ++zz) {
            for (int xx=x; xx<xMax; ++xx) {
    
                int nodeIndex = xx + zz * xSegments;
                
                v0.x = xx * segmentSize.x;
                v0.y = nodes[nodeIndex].height;
                v0.z = zz * segmentSize.y;
                
                v1.x = v0.x + segmentSize.x;
                v1.y = nodes[nodeIndex + 1].height;
                v1.z = v0.z;
                
                v2.x = v1.x;
                v2.y = nodes[nodeIndex + 1 + xSegments].height;
                v2.z = v0.z + segmentSize.y;
                
                if (IVertexMesh::RayIntersectsTriangle(ray, v0, v2, v1, &distance, &u, &v)) {
                    if (distance<intersectData.minDistance) {
                        intersectData.minDistance = distance;
                        intersectData.barycentricU = u;
                        intersectData.barycentricV = v;
                        intersectData.hit = true;
                        intersectData.triangleIndex = nodeIndex;
                        intersectData.v0 = v0;
                        intersectData.v1 = v2;
                        intersectData.v2 = v1;
                    }
                } else {
                    
                    v1.x = v0.x;
                    v1.y = nodes[nodeIndex + xSegments].height;
                    v1.z = v2.z;
                    
                    if (IVertexMesh::RayIntersectsTriangle(ray, v0, v1, v2, &distance, &u, &v)) {
                        if (distance<intersectData.minDistance) {
                            intersectData.minDistance = distance;
                            intersectData.barycentricU = u;
                            intersectData.barycentricV = v;
                            intersectData.hit = true;
                            intersectData.triangleIndex = nodeIndex;
                            intersectData.v0 = v0;
                            intersectData.v1 = v1;
                            intersectData.v2 = v2;
                        }
                    }
                    
                }
            }
        }
   
    } else {
        width /= 2;
        depth /= 2;
        --level;
        IntersectQuad(x, z,                         widthHalf, depthHalf, level, ray, intersectData);
        IntersectQuad(x + widthHalf, z,             widthHalf, depthHalf, level, ray, intersectData);
        IntersectQuad(x,             z + depthHalf, widthHalf, depthHalf, level, ray, intersectData);
        IntersectQuad(x + widthHalf, z + depthHalf, widthHalf, depthHalf, level, ray, intersectData);
    }
}
