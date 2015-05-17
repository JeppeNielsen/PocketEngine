//
//  ParticleMeshUpdater.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/14/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"
#include "ParticleEmitter.hpp"
#include "Mesh.hpp"

namespace Pocket {
    class ParticleMeshUpdater : public GameSystem {
    public:
        void Initialize();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        
        void Update(float dt);
    private:
        void UpdateMesh(Mesh* mesh, ParticleEmitter* emitter);
        
    };
}