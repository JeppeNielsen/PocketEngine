//
//  HydralicSystem.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 02/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "HydralicSystem.h"
#include "MathHelper.hpp"

void HydralicSystem::ObjectAdded(Pocket::GameObject *object) {
   
}

void HydralicSystem::ObjectRemoved(Pocket::GameObject *object) {

}

void HydralicSystem::Update(float dt) {
    for(GameObject* go : Objects()) {
        Hydralic* hydralic = go->GetComponent<Hydralic>();
        Spring* spring = go->GetComponent<Spring>();
        if (hydralic->speed>-0.001f && hydralic->speed<0.001f) continue;
        
        float targetLength = spring->length + dt * hydralic->speed;
        targetLength = MathHelper::Clamp(targetLength, hydralic->minLength, hydralic->maxLength);
        spring->length = targetLength;
    }
}