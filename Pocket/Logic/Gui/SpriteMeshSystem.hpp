//
//  SpriteMeshSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameSystem.hpp"
#include "Mesh.hpp"
#include "Sizeable.hpp"
#include "Sprite.hpp"

namespace Pocket {
    class SpriteMeshSystem : public GameSystem<Sprite, Sizeable, Mesh> {
    public:
        void ObjectAdded(GameObject *object);
        void ObjectRemoved(GameObject *object);
        void UpdateMesh(GameObject* object);
    };
}