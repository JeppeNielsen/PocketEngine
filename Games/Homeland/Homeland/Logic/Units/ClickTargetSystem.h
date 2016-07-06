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
#include "Particle.h"
#include "Mappable.h"

using namespace Pocket;

struct ClickTargetSystem: public GameSystem<Touchable> {
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void ObjectClicked(TouchData d);

private:
    using Selectables = SelectableCollection<Movable, Particle, Mappable>;
    Selectables* selectables;
};