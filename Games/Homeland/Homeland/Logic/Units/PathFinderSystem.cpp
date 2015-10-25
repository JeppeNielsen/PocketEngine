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
    Particle* particle = object->GetComponent<Particle>();
    movable->path.clear();
    Timer t;
    t.Begin();
    particle->lastTriangle = map->CalculatePath(particle->position, movable->Target, movable->path);
    double time = t.End();
    movable->prevPathPosition = particle->position;
    movable->stillFrames = 0;
    movable->prevPosition = particle->position;
    
    std::cout<< " Path finding took : " << time<<std::endl;
}