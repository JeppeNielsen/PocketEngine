//
//  PieceLineDetectorSystem.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PieceLineDetectorSystem.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "PieceCollider.hpp"
#include <map>
#include "PieceVelocity.hpp"
#include "MathHelper.hpp"

void PieceLineDetectorSystem::Initialize() {
    AddComponent<Piece>();
    AddComponent<PointGravity>();
    AddComponent<PointTransform>();
}

void PieceLineDetectorSystem::AddedToWorld(Nano::GameWorld &world) {
    nodes = world.CreateSystem<PieceNodes>();
}

void PieceLineDetectorSystem::ObjectAdded(Nano::GameObject *object) {
    
}

void PieceLineDetectorSystem::ObjectRemoved(Nano::GameObject *object) {
    
}

void PieceLineDetectorSystem::SetScore(Score *score) {
    this->score = score;
}

void PieceLineDetectorSystem::Update(float dt) {
    DetectLines();
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        PointTransform* transform = object->GetComponent<PointTransform>();
        PointGravity* gravity = object->GetComponent<PointGravity>();
        
        if (gravity->movedDownThisFrame) {
            if (gravity->lastPosition.y == transform->position().y) {
                AddPointsToCheck(object);
            }
        }
    }
}

void PieceLineDetectorSystem::AddPointsToCheck(Nano::GameObject *object) {
    Piece* piece = object->GetComponent<Piece>();
    PointTransform* transform = object->GetComponent<PointTransform>();
    
    for (int x = 0; x<4; x++) {
        for (int y=0; y<4; y++) {
            if (piece->grid[x][y]) {
                Point worldPosition = transform->LocalToWorld(Point(x-piece->pivotX, y-piece->pivotY));
                pointsToCheck.push_back(worldPosition);
            }
        }
    }
}

void PieceLineDetectorSystem::DetectLines() {
    if (pointsToCheck.empty()) return;
    int minY = 100000;
    int maxY = -100000;
    
    typedef std::map<int, int> XPositions;
    XPositions xpositions;
    
    for (int i=0; i<pointsToCheck.size(); i++) {
        Point& worldPosition = pointsToCheck[i];
        if (worldPosition.y>maxY) maxY = worldPosition.y;
        if (worldPosition.y<minY) minY = worldPosition.y;
        xpositions[worldPosition.y] = worldPosition.x;
    }
    pointsToCheck.clear();
    
    int linesCounter = 0;
    
    for (XPositions::iterator it = xpositions.begin(); it!=xpositions.end(); ++it) {
        ObjectCollection piecesAcross;
        Point p;
        p.y = it->first;
        p.x = it->second;
        piecesAcross.push_back(nodes->FindNodeAtPosition(p));
        FindAcross(p, Point(-1,0), piecesAcross);
        FindAcross(p, Point(1,0), piecesAcross);
        
        if (piecesAcross.size()==9) {
            
            for (int i=0; i<piecesAcross.size(); i++) {
                piecesAcross[i]->RemoveComponent<Piece>();
                piecesAcross[i]->RemoveComponent<PointTransform>();
                PieceVelocity* velocity = piecesAcross[i]->AddComponent<PieceVelocity>();
                velocity->velocity = Vector3(piecesAcross[i]->GetComponent<PointTransform>()->position().x * MathHelper::Random(2, 4),20,20);
                velocity->gravity = Vector3(0,-100,0);
                velocity->angularVelocity = Vector3(MathHelper::Random(-5, 5),MathHelper::Random(-5, 5),MathHelper::Random(-5, 5));
            }
            
            ObjectCollection piecesToMoveDown;
            nodes->FindNodesAbovePosition(p.y, piecesToMoveDown);
            for (int i=0; i<piecesToMoveDown.size(); i++) {
                if (piecesToMoveDown[i]->GetComponent<PointGravity>()) {
                    continue;
                }
                //piecesToMoveDown[i]->GetComponent<Piece>()->Print();
                piecesToMoveDown[i]->GetComponent<PieceCollider>()->AddMovement(Point(0,-1), 0, true);
            }
            
            linesCounter++;
        }
    }
    
    score->AddLines(linesCounter);
}

void PieceLineDetectorSystem::FindAcross(Nano::Point startPosition, Nano::Point delta, ObjectCollection &piecesAcross) {
    
    Point position = startPosition;
    while (true) {
        position += delta;
        GameObject* piece = nodes->FindNodeAtPosition(position);
        if (piece) {
            piecesAcross.push_back(piece);
        } else {
            break;
        }
    }
}

void PieceLineDetectorSystem::PieceNodes::Initialize() {
    AddComponent<Piece>();
    AddComponent<PointTransform>();
}

void PieceLineDetectorSystem::PieceNodes::ObjectAdded(Nano::GameObject *object) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* oo = *it;
        if (!oo->GetComponent<Piece>()) {
            int je=0;
            je++;
        }
    }
}

void PieceLineDetectorSystem::PieceNodes::ObjectRemoved(Nano::GameObject *object) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* oo = *it;
        if (!oo->GetComponent<Piece>()) {
            int je=0;
            je++;
        }
    }
}

GameObject* PieceLineDetectorSystem::PieceNodes::FindNodeAtPosition(const Nano::Point &position) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        Piece* piece = object->GetComponent<Piece>();
        if (piece->wall) continue;
        PointTransform* transform = object->GetComponent<PointTransform>();
        if (transform->position() == position) return object;
    }
    return 0;
}

void PieceLineDetectorSystem::PieceNodes::FindNodesAbovePosition(int y, ObjectCollection& objects) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        Piece* piece = object->GetComponent<Piece>();
        if (piece->wall) continue;
        PointTransform* transform = object->GetComponent<PointTransform>();
        if (transform->position().y>y)  {
            objects.push_back(object);
        }
    }
}
