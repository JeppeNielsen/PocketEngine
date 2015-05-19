//
//  PieceStyle.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PieceStyle.hpp"


void PieceStyle::Reset() {
    color = Colour::White();
}

void PieceStyle::Clone(const PieceStyle &other) {
    color = other.color;
}