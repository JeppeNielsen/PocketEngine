//
//  BlockPatchTransformer.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 8/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "BlockPatchTransformer.hpp"
#include "Draggable.hpp"

void BlockPatchTransformer::Initialize() {
    AddComponent<Block>();
    AddComponent<Transform>();
}

void BlockPatchTransformer::ObjectAdded(GameObject* object) {
    object->GetComponent<Block>()->Moving.Changed += event_handler(this, &BlockPatchTransformer::MovingChanged, object);
}

void BlockPatchTransformer::ObjectRemoved(GameObject* object) {
    object->GetComponent<Block>()->Moving.Changed -= event_handler(this, &BlockPatchTransformer::MovingChanged, object);
}

void BlockPatchTransformer::MovingChanged(Block *block, GameObject* object) {

        Vector3 worldPosition = object->GetComponent<Transform>()->World.GetValue()->TransformPosition(0);
        
        if (block->Patch && !block->Moving()) {
            object->Parent = block->Patch();
            object->GetComponent<Transform>()->Position = 0;
            
            
            
            
            //block->Owner()->GetComponent<Transform>()->Rotation =
        } else {
            Vector3 localPosition = camera->GetComponent<Transform>()->WorldInverse.GetValue()->TransformPosition(worldPosition);
            localPosition.z = -13;
            
            object->Parent = camera;
            object->GetComponent<Transform>()->Position = localPosition;
        }
    
    
    if (block->Moving) {
        MovingBlock& movingBlock = movingBlocks[block];
        movingBlock.object = object;
        movingBlock.upInParentSpace = object->GetComponent<Transform>()->Local.GetValue()->TransformVector({0,1,0});
        movingBlock.worldUp = object->GetComponent<Transform>()->World.GetValue()->TransformVector({0,1,0});
        movingBlock.block = block;
    } else {
        auto it = movingBlocks.find(block);
        if (it!=movingBlocks.end()) {
            movingBlocks.erase(it);
        }
    }
}


void BlockPatchTransformer::Update(float dt) {
    
    for (MovingBlocks::iterator it = movingBlocks.begin(); it!=movingBlocks.end(); ++it) {
        UpdateMovingBlock(it->second, dt);
    }
}

void BlockPatchTransformer::UpdateMovingBlock(MovingBlock& movingBlock, float dt) {
    Block* block = movingBlock.block;
    GameObject* object = movingBlock.object;
    if (block->Patch) {
        GameObject* parent = object->Parent;
        
        Transform* transform = object->GetComponent<Transform>();
        
        Vector3 patchWorldPosition = block->Patch()->GetComponent<Transform>()->World.GetValue()->TransformPosition(0);
        Vector3 patchWorldNormal = block->Patch()->GetComponent<Transform>()->World.GetValue()->TransformVector({0,0,1});
        Vector3 patchWorldUp = block->Patch()->GetComponent<Transform>()->World.GetValue()->TransformVector({0,1,0});
        
        
        Vector3 position = parent->GetComponent<Transform>()->WorldInverse.GetValue()->TransformPosition(patchWorldPosition);
        Vector3 normal = parent->GetComponent<Transform>()->WorldInverse.GetValue()->TransformVector(patchWorldNormal);
        Vector3 up = parent->GetComponent<Transform>()->WorldInverse.GetValue()->TransformVector(patchWorldUp);
        
        transform->Position = position;
        
        Vector3 worldUpInPatchSpace = object->GetComponent<Transform>()->WorldInverse.GetValue()->TransformVector(movingBlock.worldUp);
        worldUpInPatchSpace.z = 0;
        //float angle = atan2f(worldUpInPatchSpace.y, worldUpInPatchSpace.x);
        //block->Rotation = ((int)roundf(angle / M_PI_2))-1;
        
        //float angle = atan2f(-worldUpInPatchSpace.y, worldUpInPatchSpace.x);
        //float angle = atan2f(worldUpInPatchSpace.y, -worldUpInPatchSpace.x);
        //float angle = atan2f(-worldUpInPatchSpace.y, -worldUpInPatchSpace.x);
        
        //float angle = atan2f(worldUpInPatchSpace.x, worldUpInPatchSpace.y);
        float angle = atan2f(-worldUpInPatchSpace.x, worldUpInPatchSpace.y);
        //float angle = atan2f(worldUpInPatchSpace.x, -worldUpInPatchSpace.y);
        //float angle = atan2f(-worldUpInPatchSpace.x, -worldUpInPatchSpace.y);
        
        block->Rotation = ((int)roundf(angle / M_PI_2));
        
        Quaternion rotation = Quaternion::LookAt(0, -normal, up) * block->RotationToQuaternion();// transform->Local.GetValue()->TransformVector({0,1,0}));
        transform->Rotation = Quaternion::SlerpExtraSpins(dt * 30, transform->Rotation, rotation, 0);
        
    } else {
        Transform* transform = object->GetComponent<Transform>();
        Vector3 position = transform->Position;
        position.z = -13;
        transform->Position = position;
    }
}
