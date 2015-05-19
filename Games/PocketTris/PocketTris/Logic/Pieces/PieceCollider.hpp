//
//  PieceCollider.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Point.hpp"
#include "Event.hpp"

using namespace Nano;

Component(PieceCollider)
public:
    struct Movement {
        Point deltaPosition;
        int deltaRotation;
        bool force;
    };

    void Reset();

    void AddMovement(const Point& deltaMovement, int deltaRotation, bool force = false);

    typedef std::vector<Movement> Movements;
    Movements movements;

    Event<PieceCollider*> Changed;

};