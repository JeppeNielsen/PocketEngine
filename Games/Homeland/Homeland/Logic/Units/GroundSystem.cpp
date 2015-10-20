//
//  MovableGroundSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 23/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GroundSystem.h"


void GroundSystem::ObjectAdded(Pocket::GameObject *object) {
    objectsMoved.Add(object->GetComponent<Transform>()->Position, object);
}

void GroundSystem::ObjectRemoved(Pocket::GameObject *object) {
    objectsMoved.Remove(object->GetComponent<Transform>()->Position, object);
}

void GroundSystem::Update(float dt) {
    for(auto o : objectsMoved.Objects()) {
        UpdateObject(dt, o);
    }
}

void GroundSystem::UpdateObject(float dt, Pocket::GameObject *object) {
    Map* map = object->GetComponent<Mappable>()->Map;
    if (!map) return;
    Transform* transform = object->GetComponent<Transform>();
    Vector3 position = transform->Position;
    
    int x = (int)floorf(position.x);
    int z = (int)floorf(position.z);
    
    float dx = position.x - x;
    float dz = position.z - z;
    
    const Map::Node& n0 = map->GetNode(x,z);
    const Map::Node& n1 = map->GetNode(x+1,z);
    const Map::Node& n2 = map->GetNode(x+1,z+1);
    const Map::Node& n3 = map->GetNode(x,z+1);
    
    float height = 0.0f;
    Vector3 normal;
    
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
        height += (n2.height - n1.height) * dz;
        height += (n2.height - n3.height) * dx;
        
        normal = n2.normal;
        normal += (n2.normal - n1.normal) * dz;
        normal += (n2.normal - n3.normal) * dx;
    }
    position.y += (height - position.y) * dt * 15.0f;
    
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
}
