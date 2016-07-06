//
//  ObstacleSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 25/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ObstacleSystem.h"
#include <unistd.h>

void ObstacleSystem::AddedToWorld(Pocket::GameWorld &world) {
    isDirty = false;
    isNavMeshReady = false;
    shouldCreateNavMesh = false;
    currentNavMesh = 0;
    version = 0;
    worker = std::thread([this] { CreateNavMeshWorker(); });
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
    if (!isNavMeshReady) {
        if (isDirty) {
            isDirty = false;
            shouldCreateNavMesh = true;
        }
    } else {
        map->SetNavMesh(&navMesh[currentNavMesh]);
        isNavMeshReady = false;
    }
}

void ObstacleSystem::UpdateMap(Map *map, Transform* transform, const Pocket::Point &size, int addition) {
    this->map = map;
    for (float z = -size.y-0.55f; z<(size.y+0.55f); z+=0.5f) {
        for (float x = -size.x-0.55f; x<(size.x+0.55f); x+=0.5f) {
            Vector3 worldPosition = transform->World().TransformPosition({x,0,z});
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

void ObstacleSystem::CreateNavMeshWorker() {
    
    while (true) {
        while (!shouldCreateNavMesh) {
            usleep(2000);
        }
        currentNavMesh = 1 - currentNavMesh;
        map->CreateNavigationMesh(navMesh[currentNavMesh]);
        navMesh[currentNavMesh].version = version++;
        shouldCreateNavMesh = false;
        isNavMeshReady = true;
    }
}
