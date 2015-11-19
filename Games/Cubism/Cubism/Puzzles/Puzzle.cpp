//
//  Puzzle.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Puzzle.hpp"

Puzzle::Puzzle() {}
Puzzle::~Puzzle() {}

bool Puzzle::Intersects(const Pocket::Point3 &point) const {
    for (size_t i = 0; i<points.size(); i++) {
        if (points[i] == point) {
            return true;
        }
    }
    return false;
}