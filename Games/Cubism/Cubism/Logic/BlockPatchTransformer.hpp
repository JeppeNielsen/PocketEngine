//
//  BlockPatchTransformer.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Block.hpp"
#include "Transform.hpp"
#include "Patch.hpp"

using namespace Pocket;

class BlockPatchTransformer : public GameSystem {
    
public:
    
    GameObject* camera;
    
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
    void Update(float dt);
    
private:
    
    struct MovingBlock {
        GameObject* object;
        Block* block;
        Vector3 upInParentSpace;
        Vector3 worldUp;
        Vector3 worldRight;
    };
    
    void MovingChanged(Block* block, GameObject* object);
    
    typedef std::map<Block*, MovingBlock> MovingBlocks;
    MovingBlocks movingBlocks;
    
    void UpdateMovingBlock(MovingBlock& movingBlock, float dt);
    
};