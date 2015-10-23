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
        if (particle->immovable) continue;
        Mappable* mappable = go->GetComponent<Mappable>();
        Map* map = mappable->Map;
        
        Vector2 nearestPosition = map->FindNearestValidPosition(particle->position);
        const float radius = 1.0f;
        Vector2 vector = nearestPosition - particle->position;
        float length = vector.Length();
        if (length>radius) {
            vector *= (1.0f / length);
            float penetration = length - radius;
            particle->position += vector * penetration;
        }
        //particle->position = ;
    }
}