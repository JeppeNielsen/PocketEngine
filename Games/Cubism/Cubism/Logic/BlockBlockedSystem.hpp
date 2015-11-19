//
//  BlockBlockedSystem.hpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/18/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Transform.hpp"
#include "GameSystem.hpp"
#include "Block.hpp"
#include <set>

using namespace Pocket;

class BlockBlockedSystem : public GameSystem {
    
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    
private:
    void PatchChanged(Block* block, GameObject* object);
    void RotationChanged(Block* block);
    
    void RemoveBlock(Block* block);
    
    typedef std::set<Block*> Blocks;
    Blocks blocks;
    
    bool checkForBlocked;
    Block* checkThisBlock;
    
    bool GridIntersects(Block* block);
    
};