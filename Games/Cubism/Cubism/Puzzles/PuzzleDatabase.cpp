//
//  PuzzleDatabase.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 9/14/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PuzzleDatabase.hpp"

Puzzle& PuzzleDatabase::AddPuzzle() {
    puzzles.push_back(Puzzle());
    return puzzles[puzzles.size() - 1];
}

const PuzzleCollection& PuzzleDatabase::Puzzles() { return puzzles; }