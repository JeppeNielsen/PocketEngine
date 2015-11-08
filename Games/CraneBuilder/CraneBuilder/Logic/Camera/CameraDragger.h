//
//  CameraMover.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 07/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"

using namespace Pocket;

Component(CameraDragger)
    Vector3 startDragPosition;
    Vector3 startTransformPosition;
    float zPos;
    bool isDragging;
    void Reset();
};