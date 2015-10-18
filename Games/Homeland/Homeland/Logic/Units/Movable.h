//
//  Movable.h
//  Homeland
//
//  Created by Jeppe Nielsen on 23/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Vector3.hpp"

using namespace Pocket;

Component(Movable)
public:
    Movable();
    float Speed;
    typedef std::vector<Vector3> Path;
    Path path;
    void Reset();
    Property<Movable*, Vector3> Target;

    Vector3 currentTarget;
    Vector3 prevPosition;
};
