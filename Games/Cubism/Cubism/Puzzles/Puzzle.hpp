//
//  Puzzle.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "Point3.hpp"
#include <string>

using namespace Pocket;

class Puzzle;
typedef std::vector<Puzzle> PuzzleCollection;

class Puzzle {
public:
    Puzzle();
    ~Puzzle();
    
    bool Intersects(const Point3& point) const;
    
    typedef std::vector<Point3> Points;
    Points points;
    int seed;
    std::string Name;
};

