//
//  PathFinderSystem.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 10/08/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "PathFinderSystem.h"
#include <iostream>
#include "Timer.hpp"

void PathFinderSystem::ObjectAdded(Pocket::GameObject *object) {
    objectTargetsChanged.Add(object->GetComponent<Movable>()->Target, object);
    
}

void PathFinderSystem::ObjectRemoved(Pocket::GameObject *object) {
    objectTargetsChanged.Remove(object->GetComponent<Movable>()->Target, object);
}

void PathFinderSystem::Update(float dt) {
    auto objects = objectTargetsChanged.Objects();
    for(auto object : objects) {
        CalculatePath(object);
    }
    objectTargetsChanged.Clear();
}

void PathFinderSystem::CalculatePath(Pocket::GameObject *object) {
    Map* map = object->GetComponent<Mappable>()->Map;
    Movable* movable = object->GetComponent<Movable>();
    Transform* transform = object->GetComponent<Transform>();
    Vector3 position = transform->Position;
    Vector3 target = movable->Target;
    
    Point start = { (int)position.x, (int)position.z};
    Point end = { (int)target.x, (int)target.z };
    movable->path.clear();
    Timer t;
    t.Begin();
    map->CalculatePath(start, end, movable->path);
    double time = t.End();
    std::cout<< " Path finding took : " << time<<std::endl;
}