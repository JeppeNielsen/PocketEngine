//
//  Block.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/17/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameComponent.hpp"
#include "Vector2.hpp"
#include "Quaternion.hpp"
#include "Property.hpp"
#include "Point3.hpp"
#include "Property.hpp"
using namespace Pocket;

Component(Block)
public:

    Block();
    void Reset();

    Vector2 GetGridPosition(int gridIndex);
    Vector2 GetGridNormal(int gridIndex);
    bool GetGrid(int gridIndex);
    void SetGrid(int gridIndex, bool value);

    Property<Block*, int> Rotation;
    Quaternion RotationToQuaternion();
    float RotationTimer;

    Vector3 GetGridWorldPosition(int gridIndex, const Matrix4x4& world);

    void CreateRandom();

    bool PointIntersects(const Vector3& point, const Matrix4x4& inverseWorld);

    void EnsureCornersAreValid();

    Property<Block*, GameObject*> Patch;
    Property<Block*, bool> Moving;
    Property<Block*, bool> Blocked;

private:
    bool grid[16];

};