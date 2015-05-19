//
//  Piece.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include <string>

using namespace Pocket;

Component(Piece)
public:

    void Reset();
    void Clone(const Piece& other);

    void SetFromString(std::string s);

    bool grid[4][4];
    int pivotX;
    int pivotY;

    bool wall;

    void Print();

};



