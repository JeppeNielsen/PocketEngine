//
//  ObstacleSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 25/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ObstacleSystem.h"

void ObstacleSystem::AddedToWorld(Pocket::GameWorld &world) {
    isDirty = false;
}

void ObstacleSystem::ObjectAdded(Pocket::GameObject *object) {
    isDirty = true;
    UpdateMap(object->GetComponent<Mappable>()->Map, object->GetComponent<Transform>(), object->GetComponent<Obstacle>()->size, 1);
}

void ObstacleSystem::ObjectRemoved(Pocket::GameObject *object) {
    isDirty = true;
    UpdateMap(object->GetComponent<Mappable>()->Map, object->GetComponent<Transform>(), object->GetComponent<Obstacle>()->size, -1);
}

void ObstacleSystem::Update(float dt) {
    if (!isDirty) return;
    isDirty = false;
    map->CreateNavigationMesh();
}

void ObstacleSystem::UpdateMap(Map *map, Transform* transform, const Pocket::Point &size, int addition) {
    this->map = map;
    for (float z = -size.y-0.5f; z<(size.y+0.5f); z++) {
        for (float x = -size.x-0.5f; x<(size.x+0.5f); x++) {
            Vector3 worldPosition = transform->World.GetValue()->TransformPosition({x,0,z});
            int nodeX = (int)floorf(worldPosition.x);
            int nodeZ = (int)floorf(worldPosition.z);
            Map::Node& node = map->GetNode(nodeX, nodeZ);
            node.obstacles+=addition;
        }
    }
    
    Vector3 transformPosition = transform->Position;
                transformPosition.y = 1.0f;//node.height;
                transform->Position = transformPosition;
}