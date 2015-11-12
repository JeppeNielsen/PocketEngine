//
//  GridSystem.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 12/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GridSystem.h"

void GridSystem::ObjectAdded(Pocket::GameObject *object) {

}

void GridSystem::ObjectRemoved(Pocket::GameObject *object) {


}

void GridSystem::Update(float dt) {
    for(GameObject* go : Objects()) {
        Transform* transform = go->GetComponent<Transform>();
        Grid* grid = go->GetComponent<Grid>();
        Vector3 pos = transform->Position;
        pos.x = roundf(pos.x / grid->Size.x) * grid->Size.x;
        pos.y = roundf(pos.y / grid->Size.y) * grid->Size.y;
        transform->Position = pos;
    }
}