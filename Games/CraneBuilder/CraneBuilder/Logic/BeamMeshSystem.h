//
//  SpringMeshSystem.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Spring.h"
#include "Mesh.hpp"
#include "Beam.h"
#include "Atlas.hpp"

using namespace Pocket;

class BeamMeshSystem : public GameSystem<Spring, Mesh, Atlas, Beam> {
public:
    void Update(float dt);
    void ObjectAdded(GameObject* object);
};