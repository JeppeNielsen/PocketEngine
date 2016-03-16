//
//  SheetMovementSystem.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/24/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "VelocitySystem.hpp"

using namespace Pocket;

void VelocitySystem::Update(float dt) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        Velocity* velocity = object->GetComponent<Velocity>();
        Transform* transform = object->GetComponent<Transform>();
        
        if (velocity->Friction>0) {
            velocity->velocity = Vector3::Lerp(velocity->velocity, Vector3(0,0,0), velocity->Friction * dt);
        }
        
        if (velocity->MinimumSpeedBeforeStop>0) {
            if (velocity->velocity.Length()<velocity->MinimumSpeedBeforeStop) {
                velocity->velocity = Vector3::Lerp(velocity->velocity, Vector3(0,0,0), 30.0f * dt);
            }
        }
        
        transform->Position += velocity->velocity * dt;
    }
}