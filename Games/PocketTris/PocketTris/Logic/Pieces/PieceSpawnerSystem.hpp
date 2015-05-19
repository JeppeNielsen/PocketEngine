//
//  PieceSpawnerSystem.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Piece.hpp"
#include "PieceController.hpp"
#include "PieceStyle.hpp"

using namespace Nano;

class PieceSpawnerSystem : public GameSystem {
public:
    
    void CreatePieces();
    void Initialize();
    void Update(float dt);
    
private:
    void CreatePiece(std::string s, Colour color);
    
    ObjectCollection pieces;
    
};