//
//  PieceCollisionSystem.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "PointTransform.hpp"
#include "Piece.hpp"
#include "PieceCollider.hpp"
#include <set>

using namespace Nano;

class PieceCollisionSystem : public GameSystem {
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
    void Update(float dt);
    
    void HandleCollision(GameObject* object);
    
private:
    
    void ColliderChanged(PieceCollider* collider, GameObject* object);
    
    typedef std::set<GameObject*> ChangedColliders;
    ChangedColliders changedColliders;
    
    typedef std::vector<Point> CollisionPoints;
    
    bool HitAny(CollisionPoints& collisionPoints, GameObject* excludeObject);
    
};