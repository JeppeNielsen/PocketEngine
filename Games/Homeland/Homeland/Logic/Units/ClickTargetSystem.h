//
//  ClickDestinationSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 11/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Selectable.hpp"
#include "Movable.h"
#include "SelectableCollection.hpp"
#include "Touchable.hpp"

using namespace Pocket;

SYSTEM(ClickTargetSystem, Touchable)
public:
    void AddedToWorld(GameWorld& world);
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void ObjectClicked(TouchData d);

private:
    SelectableCollection* selectables;
};