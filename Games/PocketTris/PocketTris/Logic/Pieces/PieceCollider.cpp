//
//  PieceCollider.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PieceCollider.hpp"

void PieceCollider::Reset() {
    movements.clear();
}

void PieceCollider::AddMovement(const Point& deltaMovement, int deltaRotation, bool force) {
    Movement m;
    m.deltaPosition = deltaMovement;
    m.deltaRotation = deltaRotation;
    m.force = force;
    movements.push_back(m);
    Changed(this);
}