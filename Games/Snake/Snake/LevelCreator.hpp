//
//  LevelCreator.hpp
//  Snake
//
//  Created by Jeppe Nielsen on 03/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Point.hpp"
#include "Span.hpp"
#include <set>

using namespace Pocket;

class LevelCreator {

public:
    void CreateCircular(int numCirles, Span<int> radius);
    
    
    
    void AddCircle(Point center, int radius);

    const std::set<Point>& Points();

private:
    std::set<Point> points;
};