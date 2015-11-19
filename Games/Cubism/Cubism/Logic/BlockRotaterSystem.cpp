//
//  BlockRotaterSystem.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 7/18/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "BlockRotaterSystem.hpp"
#include "Transform.hpp"
#include <iostream>

void BlockRotatorSystem::Initialize() {
    AddComponent<Touchable>();
    AddComponent<Transform>();
}

void BlockRotatorSystem::ObjectAdded(Pocket::GameObject *object) {
    GameObject* parent = object->Parent();
    object->GetComponent<Touchable>()->Click += event_handler(this, &BlockRotatorSystem::BlockClicked, parent);
    object->GetComponent<Touchable>()->Down += event_handler(this, &BlockRotatorSystem::BlockDown, parent);
    object->GetComponent<Touchable>()->Up += event_handler(this, &BlockRotatorSystem::BlockUp, parent);
}

void BlockRotatorSystem::ObjectRemoved(Pocket::GameObject *object) {
    GameObject* parent = object->Parent();
    object->GetComponent<Touchable>()->Click -= event_handler(this, &BlockRotatorSystem::BlockClicked, parent);
    object->GetComponent<Touchable>()->Down -= event_handler(this, &BlockRotatorSystem::BlockDown, parent);
    object->GetComponent<Touchable>()->Up -= event_handler(this, &BlockRotatorSystem::BlockUp, parent);
    DragDistances::iterator it = dragDistances.find(parent);
    if (it!=dragDistances.end()) {
        dragDistances.erase(it);
    }
}

void BlockRotatorSystem::BlockClicked(Pocket::TouchData touch, Pocket::GameObject *object) {
    if (touch.Index!=0) return;
    DragDistances::iterator it = dragDistances.find(object);
    if (it!=dragDistances.end()) {
        if (it->second.distance<=0.2f && it->second.timer<0.5f) {
            object->GetComponent<Block>()->Rotation += 1;
            object->GetComponent<Block>()->RotationTimer = 0.2f;
        }
    }
}

void BlockRotatorSystem::BlockDown(Pocket::TouchData touch, Pocket::GameObject *object) {
    dragDistances[object] = {0, object->GetComponent<Transform>()->Position, object->GetComponent<Block>()->Patch() ? object->GetComponent<Block>()->Patch()->GetComponent<Patch>() : 0, 0 };
}


void BlockRotatorSystem::BlockUp(Pocket::TouchData touch, Pocket::GameObject *object) {
    DragDistances::iterator it = dragDistances.find(object);
    if (it!=dragDistances.end()) {
        dragDistances.erase(it);
    }
}

void BlockRotatorSystem::Update(float dt) {
    for (DragDistances::iterator it = dragDistances.begin(); it!=dragDistances.end(); ++it) {
        Vector3 current = it->first->GetComponent<Transform>()->Position;
        it->second.distance += (current - it->second.lastPosition).Length();
        it->second.lastPosition = current;
        Patch* patch = it->first->GetComponent<Block>()->Patch() ? it->first->GetComponent<Block>()->Patch()->GetComponent<Patch>() : 0;
        //std::cout<< it->second.distance << std::endl;
        if (it->second.lastPatch!=patch) {
            it->second.distance += 1.0f;
        }
        it->second.timer += dt;
    }
}



