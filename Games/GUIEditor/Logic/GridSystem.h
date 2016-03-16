//
//  GridSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 15/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"
#include "Gridable.h"
#include "PropertyListener.hpp"
#include "Sizeable.hpp"
#include "Draggable.hpp"

using namespace Pocket;

class GridSystem : public GameSystem<Transform, Draggable> {
public:
    void Initialize(GameWorld* world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    static Vector2 Size;
private:
    PropertyListener<GameObject*> changedPositions;
};