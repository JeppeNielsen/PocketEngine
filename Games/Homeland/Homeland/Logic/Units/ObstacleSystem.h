//
//  ObstacleSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 25/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Obstacle.h"
#include "Transform.hpp"
#include "GameWorld.hpp"
#include "Mappable.h"
#include <thread>

struct ObstacleSystem : public GameSystem<Transform, Obstacle, Mappable> {
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
    void UpdateMap(Map* map, Transform* transform, const Point& size, int addition);
    Map* map;
    bool isDirty;
    std::thread worker;
    bool isNavMeshReady;
    bool shouldCreateNavMesh;
    void CreateNavMeshWorker();
    NavMesh navMesh[2];
    int currentNavMesh;
    int version;
};