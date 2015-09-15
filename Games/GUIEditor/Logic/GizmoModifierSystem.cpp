//
//  GizmoModifierSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 14/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GizmoModifierSystem.h"
#include "Transform.hpp"
#include "Draggable.hpp"

void GizmoModifierSystem::ObjectAdded(Pocket::GameObject *object) {
    for (int i=0; i<8; ++i) {
        GameObject* corner = object->GetComponent<Gizmo>()->corners[i];
        changedPositions.Add(corner->GetComponent<Transform>()->Position, { object, corner, i });
    }
}

void GizmoModifierSystem::ObjectRemoved(Pocket::GameObject *object) {
    for (int i=0; i<8; ++i) {
        GameObject* corner = object->GetComponent<Gizmo>()->corners[i];
        changedPositions.Remove(corner->GetComponent<Transform>()->Position, { object, corner, i });
    }
}

void GizmoModifierSystem::Update(float dt) {
    if (changedPositions.Objects().empty()) return;
    std::cout << "  " <<changedPositions.Objects().size()<<std::endl;
    for(auto& corner : changedPositions.Objects()) {
        if (!corner.corner->GetComponent<Draggable>()->IsDragging) {
            continue;
        }
        
        corner.gizmo->GetComponent<Sizeable>()->Size = corner.corner->GetComponent<Transform>()->Position();
        
    }
    changedPositions.Clear();
}