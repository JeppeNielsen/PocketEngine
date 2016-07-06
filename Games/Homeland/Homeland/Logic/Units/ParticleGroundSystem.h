//
//  MovableGroundSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 23/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"
#include "Mappable.h"
#include "Groundable.h"
#include "PropertyListener.hpp"
#include "Particle.h"

using namespace Pocket;

struct ParticleGroundSystem : public GameSystem<Transform, Mappable, Groundable, Particle> {
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
    void UpdateObject(float dt, GameObject* object);
    PropertyListener<GameObject*> objectsMoved;
};