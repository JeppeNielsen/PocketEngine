//
//  ParticleCollisionSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 23/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ParticleCollisionSystem.h"
#include <cmath>
#include <iostream>

void ParticleCollisionSystem::Initialize() {
    AddComponent<Particle>();
}

void ParticleCollisionSystem::Update(float dt) {
    for (int i=0; i<Objects().size(); i++) {
        Particle* a = Objects()[i]->GetComponent<Particle>();
        for (int j=i+1; j<Objects().size(); j++) {
            Particle* b = Objects()[j]->GetComponent<Particle>();
            
            const float radius = 2.0f;
            
            Vector2 vector = b->position - a->position;
            float length = vector.Length();
            if (length<radius) {
                vector *= (1.0f / length);
                float penetration = radius - length;
                a->position -= vector * penetration * 0.5f;
                b->position += vector * penetration * 0.5f;
            }
        }
    }
}