//
//  PieceCollisionSystem.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PieceCollisionSystem.hpp"
#include <vector>

void PieceCollisionSystem::Initialize() {
    AddComponent<PointTransform>();
    AddComponent<Piece>();
    AddComponent<PieceCollider>();
}

void PieceCollisionSystem::ObjectAdded(GameObject* object) {
    PieceCollider* collider = object->GetComponent<PieceCollider>();
    collider->Changed += event_handler(this, &PieceCollisionSystem::ColliderChanged, object);
}

void PieceCollisionSystem::ObjectRemoved(GameObject* object) {
    PieceCollider* collider = object->GetComponent<PieceCollider>();
    collider->Changed -= event_handler(this, &PieceCollisionSystem::ColliderChanged, object);
    ChangedColliders::iterator it = changedColliders.find(object);
    if (it!=changedColliders.end()) {
        changedColliders.erase(it);
    }
}

void PieceCollisionSystem::ColliderChanged(PieceCollider *collider, GameObject* object) {
    changedColliders.insert(object);
}

void PieceCollisionSystem::Update(float dt) {
    if (changedColliders.empty()) return;
    for (ChangedColliders::iterator it = changedColliders.begin(); it!=changedColliders.end(); ++it) {
        HandleCollision(*it);
    }
    changedColliders.clear();
}

void PieceCollisionSystem::HandleCollision(GameObject* object) {
    PointTransform* pointTransform = object->GetComponent<PointTransform>();
    Piece* piece = object->GetComponent<Piece>();
    PieceCollider* collider = object->GetComponent<PieceCollider>();
    
    CollisionPoints worldPoints;
    
    for (int i=0; i<collider->movements.size(); i++) {
        PieceCollider::Movement& m = collider->movements[i];
        
        Point prevPosition = pointTransform->position;
        int prevRotation = pointTransform->rotation;
        
        pointTransform->position += m.deltaPosition;
        pointTransform->rotation += m.deltaRotation;
        
        if (!m.force) {
            worldPoints.clear();
            for (int x=0; x<4; x++) {
                for (int y=0; y<4; y++) {
                    if (piece->grid[x][y]) {
                        worldPoints.push_back(pointTransform->LocalToWorld(Point(x-piece->pivotX,y-piece->pivotY)));
                    }
                }
            }
            
            if (HitAny(worldPoints, object)) {
                pointTransform->position = prevPosition;
                pointTransform->rotation = prevRotation;
            }
        }
    }
    
    collider->movements.clear();
}

bool PieceCollisionSystem::HitAny(CollisionPoints &collisionPoints, GameObject* excludeObject) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = (*it);
        if (object == excludeObject) continue;
        
        PointTransform* pointTransform = object->GetComponent<PointTransform>();
        Piece* piece = object->GetComponent<Piece>();
        
        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                if (piece->grid[x][y]) {
                    Point worldPoint = pointTransform->LocalToWorld(Point(x-piece->pivotX,y-piece->pivotY));
                    for (int i=0; i<collisionPoints.size(); i++) {
                        if (collisionPoints[i] == worldPoint) return true;
                    }
                }
            }
        }
    }
    return false;
}


