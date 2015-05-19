//
//  PieceStyle.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Colour.hpp"

using namespace Nano;

Component(PieceStyle)
public:
    void Reset();
    void Clone(const PieceStyle& other);

    Colour color;

};