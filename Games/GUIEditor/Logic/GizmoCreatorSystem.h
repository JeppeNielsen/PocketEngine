//
//  GizmoCreatorSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 14/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Sizeable.hpp"
#include "Touchable.hpp"
#include "Gizmo.h"

using namespace Pocket;

SYSTEM(GizmoCreatorSystem, Sizeable, Touchable)
    GizmoCreatorSystem();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void AddedToWorld(GameWorld& world);
    Property<GizmoCreatorSystem*, GameObject*> CurrentGizmo;
private:
    void ObjectClicked(TouchData touch, GameObject* object);

};