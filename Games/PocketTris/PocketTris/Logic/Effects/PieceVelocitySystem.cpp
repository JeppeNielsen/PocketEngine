//
//  VelocitySystem.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/6/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PieceVelocitySystem.hpp"

void PieceVelocitySystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<PieceVelocity>();
}

void PieceVelocitySystem::ObjectAdded(Nano::GameObject *object) {
    
}

void PieceVelocitySystem::ObjectRemoved(Nano::GameObject *object) {
    
    
}

void PieceVelocitySystem::Update(float dt) {
    
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = (*it);
        Transform* transform = object->GetComponent<Transform>();
        PieceVelocity* velocity = object->GetComponent<PieceVelocity>();
        
        velocity->velocity += velocity->gravity * dt;
        
        
        transform->Position += velocity->velocity * dt;
        transform->EulerRotation += velocity->angularVelocity * dt;
        
        if (transform->Position().y<-30) {
            object->Remove();
        }
    }

}