//
//  LevelCreator.cpp
//  Snake
//
//  Created by Jeppe Nielsen on 03/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "LevelCreator.hpp"


void LevelCreator::CreateCircular(int numCirles, Span<int> radius) {

}

void LevelCreator::AddCircle(Pocket::Point center, int radius) {
    
    int radSqr = radius*radius;
    
    for (int x=-radius; x<=radius; x++) {
        for (int y=-radius; y<=radius; y++) {
            int rad = x*x+y*y;
            if (rad<radSqr) {
                points.insert(center + Point(x,y));
            }
        }
    }
}


const std::set<Point>& LevelCreator::Points() { return points; }