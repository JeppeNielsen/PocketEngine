//
//  PieceDestructorSystem.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PieceDestructorSystem.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "PieceCollider.hpp"
#include "PieceStyle.hpp"

void PieceDestructorSystem::Initialize() {
    AddComponent<Piece>();
    AddComponent<PointGravity>();
    AddComponent<PointTransform>();
}

void PieceDestructorSystem::ObjectAdded(Nano::GameObject *object) {
    
}

void PieceDestructorSystem::ObjectRemoved(Nano::GameObject *object) {
    
}

void PieceDestructorSystem::Update(float dt) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        PointTransform* transform = object->GetComponent<PointTransform>();
        PointGravity* gravity = object->GetComponent<PointGravity>();
        
        if (gravity->movedDownThisFrame) {
           if (gravity->lastPosition.y == transform->position().y) {
                SplitPiece(object);
           }
        }
    }
}

void PieceDestructorSystem::SplitPiece(Nano::GameObject *object) {
    Piece* piece = object->GetComponent<Piece>();
    PointTransform* transform = object->GetComponent<PointTransform>();
    for (int x = 0; x<4; x++) {
        for (int y=0; y<4; y++) {
            if (piece->grid[x][y]) {
                Point worldPosition = transform->LocalToWorld(Point(x-piece->pivotX, y-piece->pivotY));
                CreatePiece(worldPosition, object);
            }
        }
    }
    object->Remove();
}

void PieceDestructorSystem::CreatePiece(Nano::Point& position, GameObject* source) {
    GameObject* go = World()->CreateObject();
    go->AddComponent<Transform>();
    go->AddComponent<Piece>()->grid[0][0] = true;
    go->AddComponent<Mesh>();
    go->AddComponent<Material>();
    go->AddComponent<PieceCollider>();
    go->AddComponent<PointTransform>()->position = position;
    go->AddComponent<PieceStyle>(source);//->color = Colour::HslToRgb(0, 1.0, 1.0, 1.0);
}