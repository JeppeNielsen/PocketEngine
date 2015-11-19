//
//  BlockMeshSystem.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/17/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Block.hpp"
#include "Mesh.hpp"

using namespace Pocket;

class BlockMeshSystem : public GameSystem {
    
public:
    void Initialize();
    
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
private:
    void CreateMesh(Block* block, Mesh* mesh);
    
};

