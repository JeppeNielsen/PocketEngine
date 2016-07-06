//
//  ParticleAirSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 28/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"
#include "Mappable.h"
#include "Airable.h"
#include "PropertyListener.hpp"
#include "Particle.h"

using namespace Pocket;

struct ParticleAirSystem : public GameSystem<Transform, Mappable, Airable, Particle> {
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
    void UpdateObject(float dt, GameObject* object);
    PropertyListener<GameObject*> objectsMoved;
};