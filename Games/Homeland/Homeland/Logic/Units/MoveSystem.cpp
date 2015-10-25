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
        if (path.size()<2) continue;
        
        Particle* particle = object->GetComponent<Particle>();
        
        Vector2 source = path[path.size()-1];
        Vector2 target = path[path.size()-2];
        
        Vector2 toTarget = target - particle->position;
        Vector2 delta = source - target;
        
        //Vector2 toNextPath = target - movable->prevPathPosition;
        //Vector2 orthogonal = Vector2(-toNextPath.y, toNextPath.x);
        float dot = delta.Dot(-toTarget);
        
        float length = toTarget.SquareLength();
        
        
        if (particle->position.EqualEpsilon(movable->prevPosition, 0.01f) && path.size()>1 && length>4*4) {
            movable->stillFrames++;
            if (movable->stillFrames>10) {
                movable->Target += Vector2(0.05f,0);
            }
        }
        movable->prevPosition = particle->position;
        
        if (length<(1.5f*1.5f) || dot<0.0f) {
            movable->prevPathPosition = target;
            path.pop_back();
        } else {
            length = sqrtf(length);
            toTarget *= 1.0f / length;
            particle->position += toTarget * movable->Speed * dt;
        }
    }
}