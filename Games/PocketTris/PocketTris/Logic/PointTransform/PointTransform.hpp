//
//  PieceTransform.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/3/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Point.hpp"
#include "Property.hpp"
using namespace Pocket;

Component(PointTransform)
public:
    PointTransform();
    Property<PointTransform*, Point> position;
    Property<PointTransform*, int> rotation;

    Point LocalToWorld(const Point& localPosition);

void Reset();
void Clone(const PointTransform& other);

};