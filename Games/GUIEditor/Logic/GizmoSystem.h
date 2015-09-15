//
//  GizmoSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 14/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"
#include "Gizmo.h"
#include "Draggable.hpp"
#include "Sizeable.hpp"
#include "PropertyListener.hpp"

using namespace Pocket;

SYSTEM(GizmoSystem, Gizmo, Sizeable)

    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
private:
    GameObject* CreateCorner(Draggable::MovementMode movementMode);
};