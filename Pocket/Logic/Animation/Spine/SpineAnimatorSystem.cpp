//
//  SpineAnimatorSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 05/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SpineAnimatorSystem.hpp"
#include "MathHelper.hpp"

using namespace Pocket;

void SpineAnimatorSystem::Initialize() {
    AddComponent<SpineAnimator>();
    AddComponent<SpineModel>();
    AddComponent<SpineSkeleton>();
}

void SpineAnimatorSystem::Update(float dt) {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        GameObject* object = *it;
        
        SpineAnimator* animator = object->GetComponent<SpineAnimator>();
        SpineAnimation* animation = animator->CurrentAnimation;
        if (!animation) continue;
        
        SpineModel* spineModel = object->GetComponent<SpineModel>();
        SpineSkeleton* skeleton = object->GetComponent<SpineSkeleton>();
        
        for (SpineAnimation::Slots::iterator it = animation->slots.begin(); it!=animation->slots.end(); ++it) {
            if (it->second.Empty()) continue;
            SpineAnimation::SlotFrame* source;
            SpineAnimation::SlotFrame* dest;
            float t;
            it->second.Get(animator->time, &source, &dest, t);
            spineModel->SetImage(it->first, source->attachment, *skeleton);
        }
        
        for (SpineAnimation::Bones::iterator it = animation->bones.begin(); it!=animation->bones.end(); ++it) {
            SpineAnimation::Bone& animationBone = it->second;
            SpineModel::Bones::iterator boneIterator = spineModel->bones.find(it->first);
            if (boneIterator == spineModel->bones.end()) {
                continue; // bone does not exist in spine model
            }
            SpineModel::Bone& bone = boneIterator->second;
           
            if (!animationBone.position.Empty()) {
                SpineAnimation::Vector2Frame* source;
                SpineAnimation::Vector2Frame* dest;
                float t;
                animationBone.position.Get(animator->time, &source, &dest, t);
                bone.transform->Position = bone.position + Vector2::Lerp(source->value, dest->value, t);
            }
            if (!animationBone.rotation.Empty()) {
                SpineAnimation::RotateFrame* source;
                SpineAnimation::RotateFrame* dest;
                float t;
                animationBone.rotation.Get(animator->time, &source, &dest, t);
                float delta = (dest->rotation - source->rotation);
                while (delta > PI)
					delta -= DOUBLE_PI;
				while (delta < -M_PI)
					delta += DOUBLE_PI;
                float rotation = bone.rotation + source->rotation + delta * t;
                bone.transform->Rotation = rotation;
            }
            if (!animationBone.scale.Empty()) {
                SpineAnimation::Vector2Frame* source;
                SpineAnimation::Vector2Frame* dest;
                float t;
                animationBone.scale.Get(animator->time, &source, &dest, t);
                Vector2 scale = Vector2::Lerp(source->value, dest->value, t);
                bone.transform->Scale = Vector3(scale.x, scale.y,1.0f);
            }
        }
        
        animator->time+=dt;
        animator->time = fmodf(animator->time, animation->endTime);
    }
}