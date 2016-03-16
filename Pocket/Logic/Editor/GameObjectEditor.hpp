//
//  GameObjectEditor.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "GameObject.hpp"

namespace Pocket {
  class GameObjectEditor {
    public:
        GameObjectEditor();
        Property<GameObject*> Object;
  };
}
