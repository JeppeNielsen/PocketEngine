//
//  SizeModifierSystem.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/22/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Selectable.hpp"
#include "SizeModifier.hpp"
#include "SizeModifierNode.hpp"
#include "Sizeable.hpp"
#include "Transform.hpp"
#include "Draggable.hpp"

class SizeModifierSystem : public GameSystem<SizeModifier, Selectable, Sizeable, Transform> {
public:
    void Initialize(GameWorld* world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
private:
    void SelectionChanged(GameObject* object);
    GameObject* CreateNode(GameObject* object, int cornerIndex, Draggable::MovementMode movementMode);
    GameObject* CreateLine(GameObject* object, int index);
    GameWorld* world;
};
