//
//  SelectedColorerSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "SelectedColorer.hpp"
#include "Colorable.hpp"
#include "Selectable.hpp"

namespace Pocket {
    SYSTEM(SelectedColorerSystem, SelectedColorer, Colorable, Selectable)
    public:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void SelectedChanged(Selectable* selectable, GameObject* object);
    };
}