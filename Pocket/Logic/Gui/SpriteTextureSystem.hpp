//
//  SpriteTextureSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Mesh.hpp"
#include "Sizeable.hpp"
#include "Sprite.hpp"
#include "Atlas.hpp"

namespace Pocket {
    class SpriteTextureSystem : public GameSystem {
    public:
        void Initialize();
    protected:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        
        void SpriteTextureChanged(Sprite* sprite, GameObject* object);
    };
}