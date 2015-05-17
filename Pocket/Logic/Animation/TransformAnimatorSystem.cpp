//
//  TransformAnimatorSystem.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TransformAnimatorSystem.hpp"
#include "Transform.hpp"

using namespace Pocket;

void TransformAnimatorSystem::Initialize() {
    AddComponent<TransformAnimator>();
    AddComponent<Transform>();
}

void TransformAnimatorSystem::ObjectAdded(Pocket::GameObject *object) {
    
}

void TransformAnimatorSystem::ObjectRemoved(Pocket::GameObject *object) {
    
}

void TransformAnimatorSystem::Update(float dt) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        TransformAnimator* animator = object->GetComponent<TransformAnimator>();
        TransformAnimation* animation = animator->CurrentAnimation;
                
        Vector3 currentValues[3];
        
        if (animation) {
            animation->GetValues(&currentValues[0], &animator->progress[0]);
        } else {
            currentValues[0]=Vector3(0,0,0);
            currentValues[1]=Vector3(0,0,0);
            currentValues[2]=Vector3(1,1,1);
        }
        
        if (animator->blendingDuration>0 && animator->blendingProgress<animator->blendingDuration && animator->previousAnimation) {
            Vector3 previousValues[3];
            animator->previousAnimation->GetValues(&previousValues[0], &animator->previousProgress[0]);
            
            float blend = animator->blendingProgress / animator->blendingDuration;
            
            currentValues[0] = Vector3::Lerp(previousValues[0], currentValues[0], blend);
            currentValues[1] = Vector3::Lerp(previousValues[1], currentValues[1], blend);
            currentValues[2] = Vector3::Lerp(previousValues[2], currentValues[2], blend);
            
        }
        
        Matrix4x4 world = Matrix4x4::IDENTITY;
        Transform* transform = object->GetComponent<Transform>();
        
        transform->Matrix = Matrix4x4::CreateTranslation(currentValues[0]) * Matrix4x4::CreateRotation(currentValues[1].y, currentValues[1].x, currentValues[1].z) * Matrix4x4::CreateScale(currentValues[2]);
        
        animator->progress[0]+=dt;
        animator->progress[1]+=dt;
        animator->progress[2]+=dt;
        
        animator->blendingProgress +=  dt;
        if (animator->blendingProgress>animator->blendingDuration) {
            animator->blendingProgress = animator->blendingDuration;
        }
        
    }
}