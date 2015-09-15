//
//  GridSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 15/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Gridable.h"
#include "PropertyListener.hpp"
#include "Sizeable.hpp"
#include "Draggable.hpp"

using namespace Pocket;

SYSTEM(GridSystem, Transform, Draggable)
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    void AddedToWorld(GameWorld& world);
    static Vector2 Size;
private:
    PropertyListener<GameObject*> changedPositions;
};