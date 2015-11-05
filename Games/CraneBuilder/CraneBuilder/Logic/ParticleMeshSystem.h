//
//  ParticleMeshSystem.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 31/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Particle.h"
#include "Mesh.hpp"
#include "Atlas.hpp"

using namespace Pocket;

SYSTEM(ParticleMeshSystem, Particle, Mesh, Atlas)
public:
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
};