//
//  ParticleMapCollisionSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 23/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ParticleMapCollisionSystem.h"

void ParticleMapCollisionSystem::Update(float dt) {
    for(GameObject* go : Objects()) {
        Particle* particle = go->GetComponent<Particle>();
        Mappable* mappable = go->GetComponent<Mappable>();
        Map* map = mappable->Map;
        
        Vector2 nearestPosition = map->FindNearestValidPosition(particle->position);
        particle->position = nearestPosition;
        continue;
        const float radius = 0.01f;
        
        Vector2 vector = nearestPosition - particle->position;
        float length = vector.Length();
        if (length>radius) {
            vector *= (1.0f / length);
            float penetration = length - radius;
            particle->position += vector * 0.5f;
        }
        //particle->position = ;
    }
}