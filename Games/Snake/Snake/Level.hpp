//
//  Level.hpp
//  Snake
//
//  Created by Jeppe Nielsen on 04/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <set>
#include "Point.hpp"

using namespace Pocket;

class Level {
public:
    void LoadFromPng(std::string pngFile);
    
    std::set<Point> blockable;
    std::set<Point> spawnable;
    std::set<Point> colorSpawnPoints[4];
    std::set<Point> allPoints;
    
    void ModifyList(std::set<Point>& list, Point amount);

};