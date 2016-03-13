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
#include <set>

namespace Pocket {
  class ColorSystem : public GameSystem<Mesh, Colorable> {
    public:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void ColorChanged(GameObject* object);
        void Update(float dt);
    private:
        std::set<GameObject*> changedColorables;
    };
}