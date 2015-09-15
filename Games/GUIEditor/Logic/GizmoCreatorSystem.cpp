//
//  GizmoCreatorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 14/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GizmoCreatorSystem.h"

GizmoCreatorSystem::GizmoCreatorSystem() : CurrentGizmo(this) { CurrentGizmo = 0; }

void GizmoCreatorSystem::AddedToWorld(Pocket::GameWorld &world) {
}

void GizmoCreatorSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click += event_handler(this, &GizmoCreatorSystem::ObjectClicked, object);
}

void GizmoCreatorSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click -= event_handler(this, &GizmoCreatorSystem::ObjectClicked, object);
}

void GizmoCreatorSystem::ObjectClicked(TouchData touch, Pocket::GameObject *object) {
    if (object == CurrentGizmo()) return;
    
    if (CurrentGizmo()) {
        CurrentGizmo()->RemoveComponent<Gizmo>();
    }
    CurrentGizmo = object;
    if (CurrentGizmo()) {
        CurrentGizmo()->AddComponent<Gizmo>();
    }
 }