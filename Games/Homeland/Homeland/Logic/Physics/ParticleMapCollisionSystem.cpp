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
        Vector2 nearestPosition;
        particle->lastTriangle = map->NavMesh().FindNearestTriangle(map->NavMesh().collision, particle->position, nearestPosition);//, particle->lastTriangle);
        particle->position = nearestPosition;
    }
}