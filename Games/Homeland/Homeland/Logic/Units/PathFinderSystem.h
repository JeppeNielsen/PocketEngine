//
//  PathFinderSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 10/08/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Mappable.h"
#include "Movable.h"
#include "Particle.h"
#include "PropertyListener.hpp"
#include "NavMesh.hpp"

using namespace Pocket;

struct PathFinderSystem : public GameSystem<Mappable, Movable, Particle> {
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    NavMesh* NavMesh;
private:
    void CalculatePath(GameObject* object);
    PropertyListener<GameObject*> objectTargetsChanged;
};