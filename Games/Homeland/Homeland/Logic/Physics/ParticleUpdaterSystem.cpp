//
//  ParticleSystem.cpp
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "ParticleUpdaterSystem.h"
#include <cmath>
#include <iostream>

void ParticleUpdaterSystem::Update(float dt) {
    dt = 0.02f;
    float damping = 0.6f;// pow(2.75, dt);
    float gravity = 0;//-60 * dt * dt;

    for (auto it = Objects().begin(); it!=Objects().end(); it++) {
        
        GameObject* object = *it;
        Particle* particle = object->GetComponent<Particle>();
        if (particle->immovable) continue;
        
        Vector2 temp = particle->position;
        
        Vector2 offset = particle->position - particle->positionOld;
        
        particle->position += offset * damping + Vector2(0,gravity);
        
        particle->positionOld = temp;
    }
    
    for (auto it = Objects().begin(); it!=Objects().end(); it++) {
        GameObject* object = *it;
        Particle* particle = object->GetComponent<Particle>();
        if (particle->immovable) {
            particle->positionOld = particle->position;
        }
    }
}


