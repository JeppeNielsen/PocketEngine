//
//  PieceSpawnerSystem.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "PointGravity.hpp"
#include "PointTransform.hpp"
#include "Piece.hpp"


class PieceDestructorSystem : public GameSystem {
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
    void Update(float dt);
    
    void SplitPiece(GameObject* object);
    
    void CreatePiece(Point& position, GameObject* source);
    
};