//
//  CharacterControllerSystem.cpp
//  Platformer
//
//  Created by Jeppe Nielsen on 27/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "CharacterControllerSystem.hpp"

void CharacterControllerSystem::Update(float dt) {
    for (int i=0; i<Objects().size(); i++) {
        RigidBody2d* rigidBody =Objects()[i]->GetComponent<RigidBody2d>();
        if (!rigidBody->body) return;
        CharacterController* characterController = Objects()[i]->GetComponent<CharacterController>();
        rigidBody->body->ApplyImpulse(b2Vec2(characterController->movement.x, characterController->movement.y), rigidBody->body->GetPosition());
        //std::cout<< characterController->movement<<std::endl;
        characterController->movement = 0;
    }
}