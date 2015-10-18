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
        
        Transform* transform = object->GetComponent<Transform>();
        Vector3& target = path.back();
        
        movable->currentTarget = Vector3::Lerp(movable->currentTarget, target, 20.0f * dt);
        
        Vector3 toTarget = movable->currentTarget - transform->Position;
        toTarget.y = 0;
        
        float length = toTarget.Length();
        if (length<0.7f) {
            path.pop_back();
        } else {
            toTarget *= 1.0f / length;
            transform->Position += toTarget * movable->Speed * dt;
        }
    }
}