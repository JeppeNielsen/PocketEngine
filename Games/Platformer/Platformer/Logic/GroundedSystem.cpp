//
//  GroundedSystem.cpp
//  Platformer
//
//  Created by Jeppe Nielsen on 29/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GroundedSystem.hpp"



void GroundedSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Collidable>()->IsColliding.Changed += event_handler(this, &GroundedSystem::IsGroundedChanged, object);
}

void GroundedSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Collidable>()->IsColliding.Changed -= event_handler(this, &GroundedSystem::IsGroundedChanged, object);
}

void GroundedSystem::Update(float dt) {

}

void GroundedSystem::IsGroundedChanged(Pocket::Collidable *collidable, Pocket::GameObject *object) {
    object->GetComponent<Groundable>()->IsGrounded = collidable->IsColliding;
    std::cout << (collidable->IsColliding() ? "Colliding" : "Free ") << std::endl;
}

