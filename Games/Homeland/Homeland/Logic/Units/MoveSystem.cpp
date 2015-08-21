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
        
        Vector3 toTarget = target - transform->Position;
        toTarget.y = 0;
        
        float length = toTarget.Length();
        if (length<0.4f) {
            path.pop_back();
        } else {
            toTarget *= 1.0f / length;
            transform->Position += toTarget * movable->Speed * dt;
        }
    }
}