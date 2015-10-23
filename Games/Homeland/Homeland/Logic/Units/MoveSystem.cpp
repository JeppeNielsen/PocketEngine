//
//  MoveSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 10/08/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "MoveSystem.h"

void MoveSystem::Update(float dt) {
    for(GameObject* object : Objects()) {
        Movable* movable = object->GetComponent<Movable>();
        Movable::Path& path = movable->path;
        if (path.empty()) continue;
        
        Particle* particle = object->GetComponent<Particle>();
        Vector2& target = path.back();
        
        Vector2 toTarget = target - particle->position;
        
        float length = toTarget.Length();
        if (length<2.0f) {
            path.pop_back();
        } else {
            toTarget *= 1.0f / length;
            particle->position += toTarget * movable->Speed * dt;
        }
    }
}