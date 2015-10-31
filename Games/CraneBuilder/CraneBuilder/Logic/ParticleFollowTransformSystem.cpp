//
//  ParticleFollowTransformSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/4/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "ParticleFollowTransformSystem.h"


void ParticleFollowTransformSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Particle>();
}

void ParticleFollowTransformSystem::Update(float dt) {

    for (auto it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        Vector3 position = object->GetComponent<Transform>()->Position;
        Particle* particle = object->GetComponent<Particle>();
        particle->position = position;
        if (particle->immovable) {
            particle->positionOld = particle->position;
        }
    }

}