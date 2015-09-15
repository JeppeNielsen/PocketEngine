//
//  SizeModifierLineSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 15/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SizeModifierLineSystem.h"
#include "Sizeable.hpp"

void SizeModifierLineSystem::Update(float dt) {
    const static Vector2 positions[] {
        {0,0.5f}, {0.5f,1.0f}, {1.0f, 0.5f}, {0.5f,0}
    };
    
    for (GameObject* object : Objects()) {
        SizeModifierLine* line = object->GetComponent<SizeModifierLine>();
        Sizeable* sizeable = object->Parent()->GetComponent<Sizeable>();
        int i=line->index;
        Transform* t = object->GetComponent<Transform>();
        t->Position = positions[i] * sizeable->Size;
        t->Scale = i==0 || i==2 ? Vector3(1, sizeable->Size().y,1) : Vector3(sizeable->Size().x, 1,1);
    }
}