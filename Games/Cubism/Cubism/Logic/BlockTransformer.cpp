//
//  BlockTransformer.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/18/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "BlockTransformer.hpp"
#include "Draggable.hpp"
#include <iostream>

void BlockTransformer::Initialize() {
    AddComponent<Transform>();
    AddComponent<Block>();
}

void BlockTransformer::Update(float dt) {
    
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        
        GameObject* object = *it;
        
        Block* block = object->GetComponent<Block>();
        
        Draggable* draggable = object->GetComponent<Draggable>();
        if (draggable && draggable->IsDragging && block->Patch) continue;
        
        
        Transform* transform = object->GetComponent<Transform>();
        
        Vector3 normal = {0,0,1};
        Vector3 up = transform->Local.GetValue()->TransformVector({0,1,0});
        
        Vector3 right = normal.Cross(up);
        up = right.Cross(normal);
        
        
        Quaternion targetRotation = block->RotationToQuaternion();
        Quaternion quat = transform->Rotation;
        //quat.Normalize();
        //targetRotation.Normalize();
        //Quaternion rot = Quaternion::SlerpExtraSpins(10.0f * dt, quat, targetRotation, 0); // THIS PRODUCES NaN (Not a number) on some rotations
        //rot.Normalize();
        Quaternion rot = Quaternion::nlerp(10.0f * dt, quat, targetRotation);
        transform->Rotation = rot;// Quaternion::SlerpExtraSpins(10.0f * dt, quat, targetRotation, 0);
        
        /*
        Hierarchy* child = object->GetComponent<Hierarchy>()->Children()[0];
        GameObject* visual = child->Owner();
        
        Vector3 pos = visual->GetComponent<Transform>()->Position;
        pos = Vector3::Lerp(pos, block->Moving() && block->Patch!=0 ? Vector3(0,0,1) : Vector3(0,0,0), 20.0f * dt);
        visual->GetComponent<Transform>()->Position = pos;
        */
        
        GameObject* visual = object->Children()[0];
        Transform* visualTransform = visual->GetComponent<Transform>();
        
        Vector3 pos = visualTransform->Position;
        pos = Vector3::Lerp(pos, (block->Moving() || block->Blocked || block->RotationTimer>0) && block->Patch!=0 ? Vector3(0,0,1) : Vector3(0,0,0), 10.0f * dt);
        visualTransform->Position = pos;
        
        Quaternion visualRotation = visualTransform->Rotation;
        visualRotation = Quaternion::nlerp(10.0f * dt, visualRotation, Quaternion::IDENTITY, true);
        visualTransform->Rotation = visualRotation;
        
        Vector3 scale = transform->Scale;
        
        transform->Scale = Vector3::Lerp(scale, block->Patch!=0 ? 1.0f : 0.8f, 10 * dt);
        
        if (block->RotationTimer>0) {
            block->RotationTimer -= dt;
        }
    }
}