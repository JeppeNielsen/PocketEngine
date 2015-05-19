//
//  PieceLineDetectorSystem.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "PointGravity.hpp"
#include "PointTransform.hpp"
#include "Piece.hpp"
#include "Score.hpp"

class PieceLineDetectorSystem : public GameSystem {
public:
    void Initialize();
    void AddedToWorld(GameWorld& world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
    void SetScore(Score* score);
    
    void Update(float dt);

private:
    
    void AddPointsToCheck(GameObject* object);
    void DetectLines();
    int CountLinePieces(const Point& position);

    void FindAcross(Point startPosition, Point delta, ObjectCollection& piecesAcross);
    
    class PieceNodes : public GameSystem {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        
        GameObject* FindNodeAtPosition(const Point& position);
        
        void FindNodesAbovePosition(int y, ObjectCollection& objects);
        
        ObjectCollection nodes;
    };

    PieceNodes* nodes;
    
    typedef std::vector<Point> PointsToCheck;
    PointsToCheck pointsToCheck;
    
    Score* score;
};