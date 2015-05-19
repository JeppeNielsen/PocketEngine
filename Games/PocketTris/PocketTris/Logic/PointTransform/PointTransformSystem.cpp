//
//  PieceMeshSystem.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/2/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//


#include "PointTransformSystem.hpp"
#include "MathHelper.hpp"

void PointTransformSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<PointTransform>();
}

void PointTransformSystem::ObjectAdded(Pocket::GameObject *object) {
    Transform* transform = object->GetComponent<Transform>();
    PointTransform* pieceTransform = object->GetComponent<PointTransform>();
    transform->Position = Vector3(pieceTransform->position().x, pieceTransform->position().y, 0);
    transform->Rotation = Quaternion(ToRadians(90 * pieceTransform->rotation), Vector3(0,0,1));
}

void PointTransformSystem::ObjectRemoved(Pocket::GameObject *object) {
    
}

void PointTransformSystem::Update(float dt) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        Transform* transform = object->GetComponent<Transform>();
        PointTransform* pieceTransform = object->GetComponent<PointTransform>();
        
        Vector3 targetPosition = Vector3(pieceTransform->position().x, pieceTransform->position().y, 0);
        Quaternion targetRotation = Quaternion(ToRadians(90 * pieceTransform->rotation), Vector3(0,0,1));
        Quaternion quat =transform->Rotation;
        quat.Normalize();
        
        transform->Rotation = Quaternion::SlerpExtraSpins(10.0f * dt, quat, targetRotation, 0);
        transform->Position = Vector3::Lerp(transform->Position, targetPosition, 10.0f * dt);
    }
}