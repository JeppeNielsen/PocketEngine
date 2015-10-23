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
    for (GameObject* go : Objects()) {
        Vector2 position = go->GetComponent<Particle>()->position;
        go->GetComponent<Transform>()->Position = { position.x, 1.5f, position.y};
    }
}