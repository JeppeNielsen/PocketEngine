//
//  PieceTransform.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/3/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PointTransform.hpp"
#include "Matrix4x4.hpp"
#include "MathHelper.hpp"
#include "Quaternion.hpp"
#include <cmath>

PointTransform::PointTransform(): position(this), rotation(this) {}

void PointTransform::Reset() {
    position = 0;
    rotation = 0;
}

void PointTransform::Clone(const PointTransform &other) {
    position = other.position;
    rotation = other.rotation;
}

Point PointTransform::LocalToWorld(const Point& localPosition) {
    
    Point p = localPosition;
    
    for (int i=0; i<rotation(); i++) {
        int temp = p.x;
        p.x = -p.y;
        p.y = temp;
    }
    p.x += position().x;
    p.y += position().y;
    
    /*
    Matrix4x4 world;
    world.InitTransform(Vector3(position().x, position().y, 0), 1.0f, Quaternion(ToRadians(90 * rotation()), Vector3(0,0,1)));
    Vector3 vectorWorldPosition = world.TransformPosition(Vector3(localPosition.x, localPosition.y, 0));
    Point p;
    p.x = (int)floor((double)vectorWorldPosition.x);
    p.y = (int)floor((double)vectorWorldPosition.y);
    */
    return p;
}