//
//  Piece.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "Piece.hpp"
#include <iostream>

void Piece::Reset() {
    for (int y=0; y<4; y++) {
        for (int x = 0; x<4; x++) {
           grid[x][y] = false;
        }
    }
    pivotX = 0;
    pivotY = 0;
    wall = false;
}

void Piece::Clone(const Piece& other) {
    for (int y=0; y<4; y++) {
        for (int x = 0; x<4; x++) {
            grid[x][y] = other.grid[x][y];
        }
    }
    pivotX = other.pivotX;
    pivotY = other.pivotY;
}

void Piece::SetFromString(std::string s) {
    
    
    for (int i=0; i<s.length(); i++) {
        int x = i % 4;
        int y = i / 4;
        if (y>3) break;
        if (s[i] == 'O') {
            pivotX = x;
            pivotY = y;
        }
        grid[x][y] = (s[i] != ' ');
    }
}

void Piece::Print() {
    
    for (int y=0; y<4; y++) {
        for (int x = 0; x<4; x++) {
            std::cout<<(grid[x][y] ? "#" : " ");
        }
        std::cout<<std::endl;
    }
    
}