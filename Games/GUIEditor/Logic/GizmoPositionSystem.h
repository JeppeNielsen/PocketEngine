//
//  GizmoPositionSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 14/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"
#include "Gizmo.h"
#include "Sizeable.hpp"
#include "PropertyListener.hpp"
#include "Transform.hpp"

using namespace Pocket;

SYSTEM(GizmoPositionSystem, Gizmo, Sizeable)

    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);

    void Update(float dt);

private:
    PropertyListener<GameObject*> resizedSizeables;
};