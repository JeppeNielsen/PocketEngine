//
//  PointGravitySystem.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PointGravitySystem.hpp"

void PointGravitySystem::Initialize() {
    AddComponent<PieceCollider>();
    AddComponent<PointTransform>();
    AddComponent<PointGravity>();
}

void PointGravitySystem::ObjectAdded(Nano::GameObject *object) {
    
}

void PointGravitySystem::ObjectRemoved(Nano::GameObject *object) {
    
}

void PointGravitySystem::Update(float dt) {
    
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = (*it);
        PointGravity* gravity = object->GetComponent<PointGravity>();
        PointTransform* transform = object->GetComponent<PointTransform>();
        gravity->lastPosition = transform->position;
        gravity->Timer += gravity->Gravity * dt;
        if (gravity->Timer>1.0f) {
            gravity->Timer = 0;
            object->GetComponent<PieceCollider>()->AddMovement(Point(0,-1), 0);
            gravity->movedDownThisFrame = true;
        } else {
            gravity->movedDownThisFrame = false;
        }
    }
}
