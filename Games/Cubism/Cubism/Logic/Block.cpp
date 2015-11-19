//
//  Block.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/17/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Block.hpp"
#include "MathHelper.hpp"
#include "Point.hpp"
#include "Patch.hpp"
#include <cmath>

using namespace Pocket;

Block::Block() : Rotation(this), Patch(this), Moving(this), Blocked(this) {}

void Block::Reset() {
    for (int i=0; i<16; i++) {
        grid[i] = false;
    }
    Rotation = 0;
    Patch = 0;
    RotationTimer = 0;
}

Vector2 Block::GetGridPosition(int gridIndex) {
    
    static Vector2 positions[] = {
        {0,2},{1,2},{2,2},
        {2,1},{2,0},{2,-1},{2,-2},
        {1,-2},{0,-2},{-1,-2},{-2,-2},
        {-2,-1},{-2,0},{-2,1},{-2,2},
        {-1,2}
    };
    
    while (gridIndex<0) {
        gridIndex += 16;
    }
    return positions[gridIndex % 16];
}

Vector2 Block::GetGridNormal(int gridIndex) {
    
    static Vector2 normals[] = {
        {0,1},{0,1},{0,1},
        {1,0},{1,0},{1,0},{1,0},
        {0,-1},{0,-1},{0,-1},{0,-1},
        {-1,0},{-1,0},{-1,0},{-1,0},
        {0,1}
    };
    
    while (gridIndex<0) {
        gridIndex += 16;
    }
    return normals[gridIndex % 16];
}

bool Block::GetGrid(int gridIndex) {
    while (gridIndex<0) {
        gridIndex += 16;
    }
    return grid[gridIndex % 16];
}

void Block::SetGrid(int gridIndex, bool value) {
    while (gridIndex<0) {
        gridIndex += 16;
    }
    grid[gridIndex % 16] = value;
}

Quaternion Block::RotationToQuaternion() {
    float rot = M_PI_2 * (float)Rotation.Get();
    return Quaternion(Vector3(0,0,rot));
}

void Block::CreateRandom() {

for (int i=0; i<16; i++) {
    SetGrid(i, MathHelper::Random() < 0.5f);
}

for (int i=0; i<16; i++) {
    bool isCorner = i==2 || i==6 || i==10 || i==14;
    if (!isCorner) continue;
    if (GetGrid(i-1) || GetGrid(i+1)) {
        SetGrid(i, MathHelper::Random() < 0.5f);
    } else {
        SetGrid(i, false);
    }
}
}

Vector3 Block::GetGridWorldPosition(int gridIndex, const Pocket::Matrix4x4 &world) {
    Vector2 position = GetGridPosition(gridIndex);
    Vector3 worldPosition = world.TransformPosition(position);
    return worldPosition;
}

bool Block::PointIntersects(const Pocket::Vector3 &point, const Pocket::Matrix4x4 &inverseWorld) {
    Vector3 localPosition = inverseWorld.TransformPositionAffine(point);
    if (localPosition.z>0.5f) return false;
    if (localPosition.z<-0.5f) return false;
    int x = (int)roundf(localPosition.x);
    int y = (int)roundf(localPosition.y);
    
    
    if (x>2) return false;
    if (x<-2)return false;
    if (y>2) return false;
    if (y<-2)return false;
    
    static Point positions[] = {
        {0,2},{1,2},{2,2},
        {2,1},{2,0},{2,-1},{2,-2},
        {1,-2},{0,-2},{-1,-2},{-2,-2},
        {-2,-1},{-2,0},{-2,1},{-2,2},
        {-1,2}
    };
    for (int i=0; i<16; i++) {
        const Point& point = positions[i];
        if (x == point.x && y==point.y) {
            if (grid[i]) {
                return  true;
            } else {
                return false;
            }
        }
    }
    
    return false;
}

void Block::EnsureCornersAreValid() {
    for (int i=0; i<16; i++) {
        bool isCorner = i==2 || i==6 || i==10 || i==14;
        if (!isCorner) continue;
        if (!(GetGrid(i-1) || GetGrid(i+1))) {
            SetGrid(i, false);
        }
    }
}



