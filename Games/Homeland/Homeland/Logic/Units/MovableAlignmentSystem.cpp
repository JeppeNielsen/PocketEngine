//
//  MovableAlignmentSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 11/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "MovableAlignmentSystem.h"

void MovableAlignmentSystem::Update(float dt) {
    for(auto object : Objects()) {
        Movable* movable = object->GetComponent<Movable>();
        Movable::Path& path = movable->path;
        if (path.empty()) continue;
        Transform* transform = object->GetComponent<Transform>();
        Vector3& target = path.back();
        Vector3 toTarget = target - transform->Position;
        toTarget.y = 0;
        if (toTarget.LengthSqaured()<0.02f) continue;
        Quaternion targetRot = Quaternion(atan2f(toTarget.x, toTarget.z), {0,1,0});
        Quaternion sourceRot = transform->Rotation;
        sourceRot.Normalize();
        targetRot = Quaternion::Slerp(4.0f * dt, sourceRot, targetRot, true);
        targetRot.Normalize();
        transform->Rotation = targetRot;
    }
}