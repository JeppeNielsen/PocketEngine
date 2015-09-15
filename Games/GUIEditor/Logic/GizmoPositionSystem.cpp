//
//  GizmoPositionSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 14/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GizmoPositionSystem.h"
#include "Draggable.hpp"

void GizmoPositionSystem::ObjectAdded(Pocket::GameObject *object) {
    resizedSizeables.Add(object->GetComponent<Sizeable>()->Size, object);
}

void GizmoPositionSystem::ObjectRemoved(Pocket::GameObject *object) {
    resizedSizeables.Remove(object->GetComponent<Sizeable>()->Size, object);
}

void GizmoPositionSystem::Update(float dt) {
    if (resizedSizeables.Objects().empty()) return;
    
    const Vector2 cornerPositions[] = {
        {0,0}, {0,0.5f},{0,1},{0.5f,1},
        {1,1}, {1,0.5f},{1,0},{0.5f,0}
    };
    
    for(GameObject* object : resizedSizeables.Objects()) {
        Gizmo* gizmo = object->GetComponent<Gizmo>();
        Sizeable* sizeable = object->GetComponent<Sizeable>();
        
        for (int i=0; i<8; ++i) {
            if (gizmo->corners[i]->GetComponent<Draggable>()->IsDragging) {
                continue;
            }
            gizmo->corners[i]->GetComponent<Transform>()->Position =
            { sizeable->Size * cornerPositions[i] ,0 };
        }
    }
    resizedSizeables.Clear();
}