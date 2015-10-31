//
//  ParticleTransformSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/3/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "ParticleTransformSystem.h"


void ParticleTransformSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Particle>();
}

void ParticleTransformSystem::Update(float dt) {

    for (auto it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        object->GetComponent<Transform>()->Position = object->GetComponent<Particle>()->position;
    }

}