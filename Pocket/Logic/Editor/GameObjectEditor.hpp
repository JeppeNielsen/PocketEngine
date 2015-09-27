//
//  GameObjectEditor.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"

namespace Pocket {
  Component(GameObjectEditor)
    public:
        GameObjectEditor();
        void Reset();
        Property<GameObjectEditor*, GameObject*> Object;
  };
}
