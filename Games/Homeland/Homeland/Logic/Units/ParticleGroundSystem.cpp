//
//  MovableGroundSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 23/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ParticleGroundSystem.h"


void ParticleGroundSystem::ObjectAdded(Pocket::GameObject *object) {
    objectsMoved.Add(object->GetComponent<Transform>()->Position, object);
}

void ParticleGroundSystem::ObjectRemoved(Pocket::GameObject *object) {
    objectsMoved.Remove(object->GetComponent<Transform>()->Position, object);
}

void ParticleGroundSystem::Update(float dt) {
    for(auto o : Objects()) {
        UpdateObject(dt, o);
    }
}

void ParticleGroundSystem::UpdateObject(float dt, Pocket::GameObject *object) {
    Map* map = object->GetComponent<Mappable>()->Map;
    Particle* particle = object->GetComponent<Particle>();
    Transform* transform = object->GetComponent<Transform>();
    Groundable* groundable = object->GetComponent<Groundable>();
    Vector2 particlePosition = particle->position;
    
    if (groundable->positions.empty()) {
        groundable->positions.push_back(particlePosition);
    } else {
        if (!particlePosition.EqualEpsilon(groundable->positions.back())) {
            groundable->positions.push_back(particlePosition);
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
    
        normal = n0.normal;
        normal += (n1.normal - n0.normal) * dx;
        normal += (n3.normal - n0.normal) * dz;

    }
    else
    {
    
        height = n2.height;
        height += (n1.height - n2.height) * (1 - dz);
        height += (n3.height - n2.height) * (1 - dx);
        
        normal = n2.normal;
        normal += (n1.normal - n2.normal) * (1-dz);
        normal += (n3.normal - n2.normal) * (1-dx);
    
       
    
        
    }
    
    normal.Normalize();
    
    transform->Position = { particlePosition.x, height, particlePosition.y };
    
    
    if (groundable->positions.size()>10) {
        Vector2 direction = 0;
        for (int i = 0; i<groundable->positions.size()-1; i++) {
            direction += (groundable->positions[i+1]- groundable->positions[i]).Normalized();
        }
        
        //Quaternion rotation2d = Quaternion(atan2f(direction.y, direction.x) - MathHelper::Pi * 0.5f, Vector3(0,1,0));
        //Quaternion normalRotation = Quaternion(0, -normal);
        
        //Quaternion target = Quaternion(atan2f(direction.y, direction.x) + MathHelper::Pi * 0.5f, normal);
        Vector3 forward = {0,0,1};
        Vector3 sideways = normal.Cross(forward);
        forward = sideways.Cross(normal);
        //sideways.Normalize();
        //forward.Normalize();
        
        Quaternion rotation2d = Quaternion(Vector3(0,atan2f(direction.x, direction.y),0));
        Quaternion normalRotation = Quaternion(sideways, -normal, forward);
        Quaternion target = normalRotation * rotation2d;
        
        
        Quaternion quat = Quaternion::Slerp(10.0f * dt, transform->Rotation, target, true);
        quat.Normalize();
        transform->Rotation = quat;
        
        groundable->positions.erase(groundable->positions.begin());
    }
    
    
    /*
    Vector3 forward = transform->World.GetValue()->TransformVector({0,0,1});
    normal.Normalize();
    
    Vector3 sideways = normal.Cross(forward);
    
    sideways.Normalize();
    forward.Normalize();
    
    Quaternion target = Quaternion::LookAt(0, forward, normal);
    Quaternion target2 = Quaternion::Slerp(dt * 4.0f, transform->Rotation, target, true);
    target2.Normalize();
    
    //transform->Rotation = target2;
    transform->Position = position;
    */
}
