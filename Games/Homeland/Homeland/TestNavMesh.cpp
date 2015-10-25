//
//  TestNavMesh.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 17/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include <stdio.h>
#include "NavMesh.hpp"

using namespace Pocket;

int main_works() {
    
    std::vector<Vector2> points {
        { 0,0 }, {5,0}, {0,5},
        { 5,0 }, {5,5}, {0,5},
        { 5,0 }, {10,0}, {5,5},
        { 10,0 }, {10,5}, {5,5},
        { 5,5 }, {10,5}, {10,10},
    };

    NavMesh navMesh;
    //navMesh.Build(points);
    
    Vector2 start(0.5f,4);
    Vector2 end(9.5f,8);

/*
    NavTriangle* startTri = navMesh.FindTriangle(start);
    NavTriangle* endTri = navMesh.FindTriangle(end);
    
    auto path = navMesh.FindPath(startTri, start, endTri, end);
    auto straightPath = navMesh.FindStraightPath(path);
  */

    return 0;
}


int main__otherTest() {
    
    std::vector<Vector2> points {
        { 0,0 }, {10,0}, {0,10},
        { 10,0 }, {10,10}, {0,10},
        { 0,0 }, {5,-5}, {10,0},
        { 0,0 }, {0,10}, {-5,5},
        { 5,-5}, {15,-5}, {10,0}
    };

    NavMesh navMesh;
    //navMesh.Build(points);
    
    Vector2 start(1,3);
    Vector2 end(9,8);
    
    /*

    NavTriangle* startTri = navMesh.FindTriangle(start);
    NavTriangle* endTri = navMesh.FindTriangle(end);
    
    auto path = navMesh.FindPath(startTri, start, endTri, end);
*/

    return 0;
}