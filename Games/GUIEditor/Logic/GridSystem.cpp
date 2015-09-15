//
//  GridSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 15/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GridSystem.h"
#include "SizeModifierNode.hpp"

Vector2 GridSystem::Size;

void GridSystem::ObjectAdded(Pocket::GameObject *object) {
    changedPositions.Add(object->GetComponent<Transform>()->Position, object);
}

void GridSystem::ObjectRemoved(Pocket::GameObject *object) {
    changedPositions.Remove(object->GetComponent<Transform>()->Position, object);
}

void GridSystem::Update(float dt) {
    if (changedPositions.Objects().empty()) return;
    for(GameObject* object : changedPositions.Objects()) {
        Draggable* draggable = object->GetComponent<Draggable>();
        if (!draggable->IsDragging) continue;
        //if (object->GetComponent<SizeModifierNode>()) continue;
        
        
        Transform* transform = object->GetComponent<Transform>();
        //Gridable* gridable = object->GetComponent<Gridable>();
        Vector3 position = transform->Position;
        position.x = roundf(position.x / Size.x) * Size.x;
        position.y = roundf(position.y / Size.y) * Size.y;
        transform->Position = position;
    }
    changedPositions.Clear();
}

void GridSystem::AddedToWorld(Pocket::GameWorld &world) {
    Size = 10;
}
