//
//  EditorSpawnerSystem.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 09/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "EditorSpawner.h"
#include "Touchable.hpp"
#include "ComponentEnablerSystem.h"

using namespace Pocket;

struct EditorSpawnerSystem: public GameSystem<EditorSpawner, Touchable, Mesh> {
public:
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    ComponentEnablerSystem* state;
private:
    void Click(TouchData d);
};