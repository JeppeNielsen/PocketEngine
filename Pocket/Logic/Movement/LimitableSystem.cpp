//
//  LimitableSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "LimitableSystem.hpp"

using namespace Pocket;

void LimitableSystem::Update(float dt) {
    for (GameObject* object : Objects()) {
        Transform* transform = object->GetComponent<Transform>();
        Limitable* limitable = object->GetComponent<Limitable>();
        Vector3 position = transform->Position;
        if (limitable->Size && limitable->View) {
            limitable->min = -limitable->Size->Size + limitable->View->Size;
            limitable->min = limitable->min.Min(limitable->max);
        }
        
        if (position.x<limitable->min.x) {
            position.x = limitable->min.x;
        } else if (position.x>limitable->max.x) {
            position.x = limitable->max.x;
        }
        
        if (position.y<limitable->min.y) {
            position.y = limitable->min.y;
        } else if (position.y>limitable->max.y) {
            position.y = limitable->max.y;
        }
        
        if (position.z<limitable->min.z) {
            position.z = limitable->min.z;
        } else if (position.z>limitable->max.z) {
            position.z = limitable->max.z;
        }
        
        transform->Position = position;
    }
}
