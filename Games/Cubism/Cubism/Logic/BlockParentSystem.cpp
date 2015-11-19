//
//  BlockRotaterSystem.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/18/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "BlockParentSystem.hpp"
#include "Transform.hpp"
#include <iostream>
#include "Patch.hpp"
#include <iostream>

void BlockParentSystem::Initialize() {
    AddComponent<Block>();
    AddComponent<Transform>();
}

void BlockParentSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Block>()->Patch.ChangedWithOld += event_handler(this, &BlockParentSystem::PatchChanged, object);
}

void BlockParentSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Block>()->Patch.ChangedWithOld -= event_handler(this, &BlockParentSystem::PatchChanged, object);
}

void BlockParentSystem::PatchChanged(Property<Block*, GameObject*>::EventData data, GameObject* object) {
    
    Block* block = data.owner;
    //Vector3 oldPosition;
    
    Vector3 patchWorldUp = object->GetComponent<Transform>()->World.GetValue()->TransformVector({0,1,0});
    
    
    if (data.Old && !data.Current) {
        //oldPosition = block->Owner()->GetComponent<Transform>()->Position;
        object->GetComponent<Transform>()->Position = {0,0,1};
    }
   
    if (data.Old) {
        //block->Owner()->GetComponent<Transform>()->Position = oldPosition;
    }
    
    
    Vector3 worldPosition = object->GetComponent<Transform>()->World.GetValue()->TransformPosition(0);
    Vector3 worldUp = object->GetComponent<Transform>()->World.GetValue()->TransformVector({0,1,0});
    
    Vector3 worldUpInParentSpace = parent->GetComponent<Transform>()->WorldInverse.GetValue()->TransformVector(worldUp);
    worldUpInParentSpace.z = 0;
    worldUpInParentSpace.Normalize();
    
    worldUp = parent->GetComponent<Transform>()->World.GetValue()->TransformVector(worldUpInParentSpace);
    
    Vector3 blockY = object->GetComponent<Transform>()->World.GetValue()->TransformVector({0,1,0});
    Vector3 blockZ = object->GetComponent<Transform>()->World.GetValue()->TransformVector({0,0,1});
    

    
    if (block->Patch) {
        object->Parent = block->Patch();
        object->GetComponent<Transform>()->Position = 0;
        object->GetComponent<Transform>()->Rotation = Quaternion::IDENTITY;
        
        Vector3 worldUpInPatchSpace = block->Patch()->GetComponent<Transform>()->WorldInverse.GetValue()->TransformVector(worldUp);
        worldUpInPatchSpace.z = 0;
        float angle = atan2f(worldUpInPatchSpace.y, worldUpInPatchSpace.x);
        //block->Rotation = ((int)roundf(angle / M_PI_2));
        
        //float angle = atan2f(-worldUpInPatchSpace.y, worldUpInPatchSpace.x);
        //float angle = atan2f(worldUpInPatchSpace.y, -worldUpInPatchSpace.x);
        //float angle = atan2f(-worldUpInPatchSpace.y, -worldUpInPatchSpace.x);
        
        //float angle = atan2f(worldUpInPatchSpace.x, worldUpInPatchSpace.y);
        //float angle = atan2f(-worldUpInPatchSpace.x, worldUpInPatchSpace.y);
        //float angle = atan2f(worldUpInPatchSpace.x, -worldUpInPatchSpace.y);
       // float angle = atan2f(-worldUpInPatchSpace.x, -worldUpInPatchSpace.y);
        
        int rotation = ((int)roundf(angle / M_PI_2))-1;
        
        block->Rotation = rotation;
        
        object->GetComponent<Transform>()->Rotation = block->RotationToQuaternion();
        
        //std::cout<<"With patch, rotation = " << rotation << std::endl;
        
        Quaternion rot = object->GetComponent<Transform>()->Rotation;
        
         //std::cout<<"Quaternion rotation = " << rot << std::endl;
        
        
        //std::cout << "Attached to patch: " << block->Patch()->name << std::endl;
        
        //std::cout << *block->Owner()->GetComponent<Transform>()->World.GetValue() << std::endl;
        
        
        //Hierarchy* child = block->Owner()->GetComponent<Hierarchy>()->Children()[0];
        //GameObject* visual = child->Owner();
        //visual->GetComponent<Transform>()->Position = Vector3(0,0,1);
        
    } else {
        
        
        
        Vector3 position = parent->GetComponent<Transform>()->WorldInverse.GetValue()->TransformPosition(worldPosition);
        //position.z = -13;
        
        object->Parent = parent;
        //block->Owner()->GetComponent<Transform>()->Position = position;
    
        
        Vector3 patchWorldUpLocal = object->Parent()->GetComponent<Transform>()->WorldInverse.GetValue()->TransformVector(patchWorldUp);
        
        patchWorldUpLocal.z = 0;
        
        float angle = atan2f(patchWorldUpLocal.y, patchWorldUpLocal.x);
        //block->Rotation = ((int)roundf(angle / M_PI_2));
        
        //float angle = atan2f(-worldUpInPatchSpace.y, worldUpInPatchSpace.x);
        //float angle = atan2f(worldUpInPatchSpace.y, -worldUpInPatchSpace.x);
        //float angle = atan2f(-worldUpInPatchSpace.y, -worldUpInPatchSpace.x);
        
        //float angle = atan2f(worldUpInPatchSpace.x, worldUpInPatchSpace.y);
        //float angle = atan2f(-worldUpInPatchSpace.x, worldUpInPatchSpace.y);
        //float angle = atan2f(worldUpInPatchSpace.x, -worldUpInPatchSpace.y);
        // float angle = atan2f(-worldUpInPatchSpace.x, -worldUpInPatchSpace.y);
        
        int rotation = ((int)roundf(angle / M_PI_2))-1;
        
        block->Rotation = rotation;
        
        std::cout<<"No patch, rotation = " << rotation << std::endl;
        
        
        
        
        object->GetComponent<Transform>()->Rotation = block->RotationToQuaternion();
    
        
        moverSystem->Update(0.001f);
    /*
        worldPosition = block->Owner()->GetComponent<Transform>()->World.GetValue()->TransformPosition(0);
       
        blockY = block->Owner()->GetComponent<Transform>()->World.GetValue()->TransformVector({0,1,0});
        blockZ = block->Owner()->GetComponent<Transform>()->World.GetValue()->TransformVector({0,0,1});
        */
    }
    
    GameObject* visual = object->Children()[0];
    
    Transform* visualTransform = visual->GetComponent<Transform>();
    
    Vector3 visualPos = visualTransform->WorldInverse.GetValue()->TransformPosition(worldPosition);
    
    visualTransform->Position = visualPos;
    
    blockY = visualTransform->WorldInverse.GetValue()->TransformVector(blockY);
    blockZ = visualTransform->WorldInverse.GetValue()->TransformVector(blockZ);
    
    visualTransform->Rotation = Quaternion::LookAt(0, -blockZ, blockY);
    
}

void BlockParentSystem::Update(float dt) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        
        if (object->GetComponent<Block>()->Patch) {
            object->GetComponent<Transform>()->Position = 0;
        }
        
    }
    
}