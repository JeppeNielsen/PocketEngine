//
//  ParticleUpdaterSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/14/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameSystem.hpp"
#include "ParticleEffect.hpp"
#include "ParticleEmitter.hpp"

namespace Pocket {
    class ParticleUpdaterSystem : public GameSystem<ParticleEffect, ParticleEmitter> {
    public:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
    private:
        void UpdateEmitter(float dt, ParticleEffect* effect, ParticleEmitter* emitter, GameObject* object);
    };
}