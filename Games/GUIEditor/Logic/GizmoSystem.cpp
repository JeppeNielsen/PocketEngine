//
//  GizmoSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 14/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GizmoSystem.h"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Touchable.hpp"
#include "Draggable.hpp"
#include <iostream>

void GizmoSystem::ObjectAdded(Pocket::GameObject *object) {
    Gizmo* gizmo = object->GetComponent<Gizmo>();
    
    for (int i=0; i<8; ++i) {
        GameObject* corner = CreateCorner(i==1 || i==5 ? Draggable::MovementMode::XAxis : (i==3 || i==7 ? Draggable::MovementMode::YAxis : Draggable::MovementMode::XYPlane));
        corner->Parent = object;
        gizmo->corners[i]=corner;
    }
}

void GizmoSystem::ObjectRemoved(Pocket::GameObject *object) {
    Gizmo* gizmo = object->GetComponent<Gizmo>();
    for (int i=0; i<8; ++i) {
        gizmo->corners[i]->Remove();
    }
}

GameObject* GizmoSystem::CreateCorner(Draggable::MovementMode movementMode) {
    GameObject* corner = World()->CreateObject();
    corner->AddComponent<Transform>()->Position = {0,0,0};
    corner->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 10, Colour::White());
    corner->AddComponent<Material>();
    corner->AddComponent<Touchable>();
    corner->AddComponent<Draggable>()->Movement = movementMode;
    return corner;
}
