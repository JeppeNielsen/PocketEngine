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
#include "Transform.hpp"
#include "PropertyListener.hpp"
#include "NavMesh.hpp"

using namespace Pocket;

SYSTEM(PathFinderSystem, Mappable, Movable, Transform)
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    NavMesh* NavMesh;
private:
    void CalculatePath(GameObject* object);
    PropertyListener<GameObject*> objectTargetsChanged;
};