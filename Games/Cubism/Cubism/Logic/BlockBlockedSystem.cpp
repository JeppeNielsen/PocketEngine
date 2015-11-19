//
//  BlockBlockedSystem.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/18/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "BlockBlockedSystem.hpp"
#include "Patch.hpp"

void BlockBlockedSystem::Initialize() {
    AddComponent<Block>();
    AddComponent<Transform>();
    checkForBlocked = false;
    checkThisBlock = 0;
}

void BlockBlockedSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Block>()->Patch.Changed += event_handler(this, &BlockBlockedSystem::PatchChanged, object);
}

void BlockBlockedSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Block>()->Patch.Changed -= event_handler(this, &BlockBlockedSystem::PatchChanged, object);
    RemoveBlock(object->GetComponent<Block>());
}

void BlockBlockedSystem::PatchChanged(Block *block, GameObject* object) {
    if (block->Patch) {
        Blocks::iterator it = blocks.find(block);
        if (it==blocks.end()) {
            blocks.insert(block);
            block->Rotation.Changed += event_handler(this, &BlockBlockedSystem::RotationChanged);
        }
        checkThisBlock = block;
    } else {
        RemoveBlock(block);
    }
    checkForBlocked = true;
}

void BlockBlockedSystem::RemoveBlock(Block *block) {
    Blocks::iterator it = blocks.find(block);
    if (it!=blocks.end()) {
        block->Rotation.Changed -= event_handler(this, &BlockBlockedSystem::RotationChanged);
        blocks.erase(it);
    }
}

void BlockBlockedSystem::RotationChanged(Block *block) {
    checkForBlocked = true;
    checkThisBlock = block;
}

void BlockBlockedSystem::Update(float dt) {
    if (!checkForBlocked) return;
    checkForBlocked = false;
    
    for (Blocks::iterator it = blocks.begin(); it!=blocks.end(); ++it) {
        Block* block = *it;
        if (block==checkThisBlock || block->Blocked) {
            block->Blocked = GridIntersects(block);
        }
    }
    checkThisBlock = 0;
}

bool BlockBlockedSystem::GridIntersects(Block* block) {

    Matrix4x4 world = (*block->Patch()->GetComponent<Transform>()->World.GetValue()) * Matrix4x4::CreateRotation(0, 0, M_PI_2 * (float)block->Rotation.Get());
    
    for (int gridIndex = 0; gridIndex<16; gridIndex++) {
        
        if (!block->GetGrid(gridIndex)) continue;
        
        Vector3 pointToTest = block->GetGridWorldPosition(gridIndex, world);
        for (Blocks::iterator it = blocks.begin(); it!=blocks.end(); ++it) {
            Block* b = *it;
            if (b==block) continue;
            if (b->Blocked) continue;
            
            Matrix4x4 worldInverse = (*b->Patch()->GetComponent<Transform>()->World.GetValue()) * Matrix4x4::CreateRotation(0, 0, M_PI_2 * (float)b->Rotation.Get());
            worldInverse = worldInverse.InvertAffine();
            
            if (b->PointIntersects(pointToTest, worldInverse)) {
                return true;
            }
        }
    }
    return false;
}


