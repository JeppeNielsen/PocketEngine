//
//  ParticleAirSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 28/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ParticleAirSystem.h"

void ParticleAirSystem::ObjectAdded(Pocket::GameObject *object) {
    objectsMoved.Add(object->GetComponent<Transform>()->Position, object);
}

void ParticleAirSystem::ObjectRemoved(Pocket::GameObject *object) {
    objectsMoved.Remove(object->GetComponent<Transform>()->Position, object);
}

void ParticleAirSystem::Update(float dt) {
    for(auto o : Objects()) {
        UpdateObject(dt, o);
    }
}

void ParticleAirSystem::UpdateObject(float dt, Pocket::GameObject *object) {
    Map* map = object->GetComponent<Mappable>()->Map;
    Particle* particle = object->GetComponent<Particle>();
    Transform* transform = object->GetComponent<Transform>();
    Airable* airable = object->GetComponent<Airable>();
    Vector2 particlePosition = particle->position;
    
    if (airable->positions.empty()) {
        airable->positions.push_back(particlePosition);
    } else {
        if (!particlePosition.EqualEpsilon(airable->positions.back(), 0.01f)) {
            airable->positions.push_back(particlePosition);
        }
    }
    
    int x = (int)floorf(particlePosition.x);
    int z = (int)floorf(particlePosition.y);
    
    float dx = particlePosition.x - x;
    float dz = particlePosition.y - z;
    
    const Map::Node& n0 = map->GetNode(x,z);
    const Map::Node& n1 = map->GetNode(x+1,z);
    const Map::Node& n2 = map->GetNode(x+1,z+1);
    const Map::Node& n3 = map->GetNode(x,z+1);
    
    float height = 0.0f;
    Vector3 normal = 0;
    
    if ((dx + dz) < 1)
    {
        height = n0.height;
        height += (n1.height - n0.height) * dx;
        height += (n3.height - n0.height) * dz;
    }
    else
    {
        height = n2.height;
        height += (n1.height - n2.height) * (1 - dz);
        height += (n3.height - n2.height) * (1 - dx);
    }
    
    if (height<1) height = 1;
    
    float targetY = height + airable->targetAboveGround;
    
    airable->currentHeightY = airable->currentHeightY + (targetY - airable->currentHeightY)* airable->heightAligmentSpeed * dt;
    
    transform->Position = { particlePosition.x, airable->currentHeightY + sinf(airable->sineWobble)*0.2f, particlePosition.y };
    airable->sineWobble += 1.0f * dt;
    
    if (airable->positions.size()>10) {
        Vector2 direction = 0;
        for (int i = 0; i<airable->positions.size()-1; i++) {
            direction += (airable->positions[i+1]- airable->positions[i]);
        }
        direction.Normalize();
        
        Vector3 normal = {0,-1,0};
        
        Vector3 forward = {0,0,1};
        Vector3 sideways = normal.Cross(forward);
        forward = sideways.Cross(normal);
        //sideways.Normalize();
        //forward.Normalize();
        
        Quaternion rotation2d = Quaternion(Vector3(0,atan2f(direction.x, direction.y) + MathHelper::Pi,0));
        Quaternion normalRotation = Quaternion(sideways, -normal, -forward);
        Quaternion target = normalRotation * rotation2d;
        
        
        Quaternion quat = Quaternion::Slerp(10.0f * dt, transform->Rotation, target, true);
        quat.Normalize();
        transform->Rotation = quat;
        
        airable->positions.erase(airable->positions.begin());
    }
}
