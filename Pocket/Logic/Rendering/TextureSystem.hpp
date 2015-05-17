//
//  TextureSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/21/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "VisibleObject.hpp"

namespace Pocket {
  class TextureSystem : public GameSystem {
        public:
            IVisibleObjectManipulator* manipulator;
            void Initialize();
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
        };
}