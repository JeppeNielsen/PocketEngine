//
//  LimitableSizeSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 30/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "LimitableSizeSystem.hpp"

void LimitableSizeSystem::Update(float dt) {
    for(auto object : Objects()) {
        Limitable* l = object->GetComponent<Limitable>();
        Sizeable* s = object->GetComponent<Sizeable>();
        l->min = -s->Size();
    }
}
