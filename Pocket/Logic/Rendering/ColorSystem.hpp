//
//  ColorSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/21/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "GameWorld.hpp"
#include "Mesh.hpp"
#include "Colorable.hpp"

namespace Pocket {
  
  class ColorSystem : public GameSystem {
        public:
            void Initialize();
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
            void ColorChanged(Colorable* colorable, GameObject* object);
            void Update(float dt);
        private:
            std::vector<GameObject*> changedColorables;
        };
  
}