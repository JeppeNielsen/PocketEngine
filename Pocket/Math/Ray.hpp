//
//  Ray.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/16/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Vector3.hpp"

namespace Pocket {
    class Matrix4x4;
    class BoundingBox;
    class Ray {
    public:
        
        Ray();
        ~Ray();
        
        Ray(const Ray& ray);
        Ray(const Vector3& position, const Vector3& direction);
        
        bool Intersect(const BoundingBox& box) const;
                
        void Transform(const Matrix4x4& matrix);
        Vector3 GetPosition(float distance) const;
        
        Vector3 position;
        Vector3 direction;
    };
}