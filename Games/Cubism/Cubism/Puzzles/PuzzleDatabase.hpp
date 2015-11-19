//
//  PuzzleDatabase.h
//  Cubism
//
//  Created by Jeppe Nielsen on 9/14/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Puzzle.hpp"

class PuzzleDatabase {
    
public:
    Puzzle& AddPuzzle();
    
    const PuzzleCollection& Puzzles();
    
private:
    PuzzleCollection puzzles;
    
    
};