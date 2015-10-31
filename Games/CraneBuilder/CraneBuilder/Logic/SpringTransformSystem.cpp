//
//  SpringTransformSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/4/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SpringTransformSystem.h"
#include "Particle.h"
#include <cmath>

void SpringTransformSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Spring>();
}

void SpringTransformSystem::Update(float dt) {

    for (auto it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        
        Spring* spring = object->GetComponent<Spring>();
        if (!spring->particleA) continue;
        if (!spring->particleB) continue;
        
        Vector3 position = (spring->particleA->position + spring->particleB->position) * 0.5f;
        
        object->GetComponent<Transform>()->Position = position;
        
        Vector3 delta = spring->particleB->position - spring->particleA->position;
        
        Quaternion rotation(atan2(-delta.x, delta.y));
        
        object->GetComponent<Transform>()->Rotation = rotation;
    }

}