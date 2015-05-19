//
//  PieceMeshSystem.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Piece.hpp"
#include "PieceStyle.hpp"
#include "Mesh.hpp"

using namespace Pocket;

class PieceMeshSystem : public GameSystem {
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
};