//
//  BlockMeshSystem.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/17/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Mesh.hpp"
#include "Patch.hpp"

using namespace Pocket;

class PatchMeshSystem : public GameSystem {
    
public:
    void Initialize();
    
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
private:
    void CreateMesh(Patch* patch, Mesh* mesh);
    
};

